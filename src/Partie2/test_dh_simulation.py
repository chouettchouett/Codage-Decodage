import unittest
import tempfile
import os
from subprocess import run, PIPE

class TestDiffieHellmanSimulation(unittest.TestCase):

    def setUp(self):
        # Créer un fichier d'entrée temporaire pour les paramètres Diffie-Hellman
        self.temp_dir = tempfile.TemporaryDirectory()
        self.input_file = os.path.join(self.temp_dir.name, "input.txt")
        self.output_file = os.path.join(self.temp_dir.name, "output.txt")

        # Écriture des paramètres Diffie-Hellman dans le fichier d'entrée
        with open(self.input_file, "w") as f:
            f.write("23\n5\n")

    def tearDown(self):
        # Nettoyer les fichiers temporaires
        self.temp_dir.cleanup()

    def test_simulation_output(self):
        """Tester que la simulation génère une clé et l'écrit dans le fichier de sortie."""
        # Exécuter le script de simulation Diffie-Hellman
        result = run(
            ["python3", "dh_genkey.py", "-i", self.input_file, "-o", self.output_file],
            stdout=PIPE,
            stderr=PIPE
        )

        # Vérifier que le script s'est exécuté sans erreurs
        self.assertEqual(result.returncode, 0, "Le script ne s'est pas exécuté avec succès.")
        self.assertTrue(os.path.exists(self.output_file), "Le fichier de sortie n'a pas été créé.")

        # Vérifier que le fichier de sortie contient une clé valide (entier)
        with open(self.output_file, "r") as f:
            key = f.read().strip()
        self.assertTrue(key.isdigit(), "La sortie ne contient pas une clé valide sous forme d'entier.")
        
    def test_shared_key_calculation(self):
        """Tester que les calculs de clés partagées d'Alice et Bob aboutissent à la même valeur."""
        # Exécuter le script et capturer la sortie
        result = run(
            ["python3", "dh_genkey.py", "-i", self.input_file, "-o", self.output_file],
            stdout=PIPE,
            stderr=PIPE
        )

        # Extraire les données depuis la sortie standard (stdout)
        stdout = result.stdout.decode()

        # Rechercher les clés dans la sortie affichée
        alice_key = None
        bob_key = None
        for line in stdout.split("\n"):
            if "Alice connait" in line:
                alice_key = int(line.split("la cle = ")[1].strip().rstrip("."))
            if "Bob connait" in line:
                bob_key = int(line.split("la cle = ")[1].strip().rstrip("."))

        # Vérifier que les deux clés sont égales
        self.assertIsNotNone(alice_key, "La clé d'Alice n'a pas été trouvée.")
        self.assertIsNotNone(bob_key, "La clé de Bob n'a pas été trouvée.")
        self.assertEqual(alice_key, bob_key, "Les clés partagées calculées par Alice et Bob ne correspondent pas.")
        
    def test_invalid_input_file(self):
        """Tester que le script gère correctement un fichier d'entrée invalide."""
        invalid_input_file = os.path.join(self.temp_dir.name, "invalid_input.txt")

        # Exécuter le script avec un fichier d'entrée inexistant
        result = run(
            ["python3", "dh_genkey.py", "-i", invalid_input_file, "-o", self.output_file],
            stdout=PIPE,
            stderr=PIPE
        )

        # Vérifier que le script échoue avec un code d'erreur approprié et un message clair
        self.assertNotEqual(result.returncode, 0, "Le script aurait dû échouer avec un fichier d'entrée invalide.")
        self.assertIn("Erreur : le fichier d'entrée n'existe pas", result.stderr.decode())

    def test_invalid_input_content(self):
        """Tester que le script gère correctement un contenu de fichier d'entrée invalide."""
        # Remplir le fichier d'entrée avec un contenu invalide
        with open(self.input_file, "w") as f:
            f.write("Contenu invalide")

        # Exécuter le script avec un contenu de fichier d'entrée invalide
        result = run(
            ["python3", "dh_genkey.py", "-i", self.input_file, "-o", self.output_file],
            stdout=PIPE,
            stderr=PIPE
        )

        # Vérifier que le script échoue avec un code d'erreur approprié et un message clair
        self.assertNotEqual(result.returncode, 0, "Le script aurait dû échouer avec un contenu d'entrée invalide.")
        self.assertIn("Erreur : le fichier d'entrée est incorrect", result.stderr.decode())


if __name__ == "__main__":
    unittest.main()
