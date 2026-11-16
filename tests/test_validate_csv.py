import unittest
from pathlib import Path
import validate_csv
import subprocess
import sys

from validate_csv import validate_row


class ValidateRowTests(unittest.TestCase):

    def test_current_csv_has_no_errors(self):
        csv_path = Path(__file__).resolve().parents[1] / "compatibility.csv"

        errors = validate_csv.validate_file(csv_path)

        self.assertEqual(errors, [])

    def test_rejects_empty_topic(self):
        row = [
            "PS5 (geral)",
            "26.07-14.22.00",
            "",
            "Nota oficial de atualizacao",
            "https://example.org/source",
            "2026-11-15",
            "confirmado",
        ]

        errors = validate_row(row)

        self.assertTrue(
            any("tema" in error.lower() for error in errors),
            "O resultado deve indicar que o campo tema está vazio.",
        )

    def test_rejects_empty_result(self):
        row = [
            "PS5 (geral)",
            "26.07-14.22.00",
            "versao oficial listada",
            "",
            "https://example.org/source",
            "2026-11-15",
            "confirmado",
        ]

        errors = validate_row(row)

        self.assertTrue(
            any("resultado" in error.lower() for error in errors),
            "O resultado deve indicar que o campo resultado está vazio.",
        )

    def test_rejects_unknown_confidence_level(self):
        row = [
            "PS5 (geral)",
            "26.07-14.22.00",
            "versao oficial listada",
            "Nota oficial de atualizacao",
            "https://example.org/source",
            "2026-11-15",
            "talvez",
        ]

        errors = validate_row(row)

        self.assertTrue(
            any("confianca" in error.lower() for error in errors),
            "O resultado deve indicar que o nível de confiança é inválido.",
        )

    def test_rejects_non_iso_date(self):
        row = [
            "PS5 (geral)",
            "26.07-14.22.00",
            "versao oficial listada",
            "Nota oficial de atualizacao",
            "https://example.org/source",
            "15/11/2026",
            "confirmado",
        ]

        errors = validate_row(row)

        self.assertTrue(
            any("data" in error.lower() for error in errors),
            "O resultado deve indicar que a data está em formato inválido.",
        )

    def test_rejects_non_https_source(self):
        row = [
            "PS5 (geral)",
            "26.07-14.22.00",
            "versao oficial listada",
            "Nota oficial de atualizacao",
            "http://example.org/source",
            "2026-11-15",
            "confirmado",
        ]

        errors = validate_row(row)

        self.assertTrue(
            any("fonte" in error.lower() for error in errors),
            "O resultado deve indicar que a fonte precisa ser HTTPS.",
        )
    def test_cli_reports_valid_csv(self):
        project_root = Path(__file__).resolve().parents[1]
        script = project_root / "validate_csv.py"
        csv_path = project_root / "compatibility.csv"

        result = subprocess.run(
            [sys.executable, str(script), str(csv_path)],
            cwd=project_root,
            capture_output=True,
            text=True,
        )

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("CSV válido", result.stdout)


if __name__ == "__main__":
    unittest.main()
