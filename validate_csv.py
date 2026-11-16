import csv
from datetime import date
from urllib.parse import urlparse


EXPECTED_HEADER = [
    "modelo",
    "versao_sistema",
    "tema",
    "resultado",
    "fonte",
    "data",
    "confianca",
]

ALLOWED_CONFIDENCE = {
    "confirmado",
    "relatado",
    "nao_verificado",
}


def validate_row(row):
    errors = []

    if len(row) != len(EXPECTED_HEADER):
        return ["A linha deve ter exatamente 7 campos."]

    if not row[2].strip():
        errors.append("O campo tema nao pode ficar vazio.")

    if not row[3].strip():
        errors.append("O campo resultado nao pode ficar vazio.")

    source = urlparse(row[4])
    if source.scheme != "https" or not source.netloc:
        errors.append("O campo fonte deve ser uma URL HTTPS válida.")

    if row[6] not in ALLOWED_CONFIDENCE:
        errors.append("O campo confianca tem um valor desconhecido.")

    try:
        parsed_date = date.fromisoformat(row[5])
        if parsed_date.isoformat() != row[5]:
            errors.append("O campo data deve usar o formato AAAA-MM-DD.")
    except ValueError:
        errors.append("O campo data deve usar o formato AAAA-MM-DD.")

    return errors


def validate_file(path):
    errors = []

    with open(path, newline="", encoding="utf-8") as csv_file:
        reader = csv.reader(csv_file)
        header = next(reader, None)

        if header != EXPECTED_HEADER:
            return ["Linha 1: o cabeçalho não corresponde ao formato esperado."]

        for line_number, row in enumerate(reader, start=2):
            for message in validate_row(row):
                errors.append(f"Linha {line_number}: {message}")

    return errors

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(
        description="Valida o arquivo CSV de compatibilidade."
    )
    parser.add_argument(
        "csv_file",
        nargs="?",
        default="compatibility.csv",
        help="Caminho do CSV (padrão: compatibility.csv).",
    )
    args = parser.parse_args()

    try:
        errors = validate_file(args.csv_file)
    except OSError as error:
        parser.error(f"Não foi possível ler o arquivo: {error}")

    if errors:
        print("CSV inválido:")
        for message in errors:
            print(f"- {message}")
        raise SystemExit(1)

    print(f"CSV válido: {args.csv_file}")
