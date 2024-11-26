import unittest
import tempfile
import os
from subprocess import run, PIPE
import re

class TestDiffieHellmanSimulation(unittest.TestCase):

    def setUp(self):
        # Créer un fichier d'entrée temporaire pour les paramètres Diffie-Hellman
        self.temp_dir = tempfile.TemporaryDirectory()
        self.input_file = os.path.join(self.temp_dir.name, "input.txt")
        self.output_file = os.path.join(self.temp_dir.name, "output.txt")
        self.log_file = "/home/chouett/Documents/Projet_avance/Codage-Decodage-main/src/Partie2/log.txt"
        self.outputmoi = "/home/chouett/Documents/Projet_avance/Codage-Decodage-main/src/Partie2/output.txt"

        # Écriture des paramètres Diffie-Hellman dans le fichier d'entrée
        with open(self.input_file, "w") as f:
            f.write("23\n5\n")

    def tearDown(self):
        # Nettoyer les fichiers temporaires
        self.temp_dir.cleanup()

    def read_log(self):
        """Lit et retourne le contenue du fichier log"""
        with open(self.log_file,"r") as f:
            return f.read()

    def test_simulation_output(self):
        """Vérifie que la simulation génère une clé et l'écrit dans le fichier de sortie."""
        result = run(
            ["python3", "/home/chouett/Documents/Projet_avance/Codage-Decodage-main/src/Partie2/dh_genkey.py", "-i", self.input_file, "-o", self.output_file],
            stdout=PIPE,
            stderr=PIPE
        )
        log_content = self.read_log()

        self.assertTrue(os.path.exists(self.outputmoi), "Le fichier de sortie n'a pas été créé.")
        with open(self.outputmoi, "r") as f:
            key = f.read().strip()
        self.assertTrue(key.isdigit(), "Le fichier de sortie ne contient pas une clé valide sous forme d'entier.")
        
    def test_shared_key_calculation(self):
        """Tester que les calculs de clés partagées d'Alice et Bob aboutissent à la même valeur."""
        result = run(
            ["python3", "/home/chouett/Documents/Projet_avance/Codage-Decodage-main/src/Partie2/dh_genkey.py", "-i", self.input_file, "-o", self.output_file],
            stdout=PIPE,
            stderr=PIPE
        )
        log_content = self.read_log()

        # Utiliser des expressions régulières pour trouver les clés
        alice_match = re.search(r"Alice.* = (\d+)", log_content)
        bob_match = re.search(r"Bob.* = (\d+)", log_content)

        alice_key = int(alice_match.group(1)) if alice_match else None
        bob_key = int(bob_match.group(1)) if bob_match else None

        # Assertions
        self.assertIsNotNone(alice_key, "La clé d'Alice n'a pas été trouvée dans stdout.")
        self.assertIsNotNone(bob_key, "La clé de Bob n'a pas été trouvée dans stdout.")
        self.assertEqual(alice_key, bob_key, "Les clés partagées calculées par Alice et Bob ne correspondent pas.")

    def test_thread_synchronization(self): 
        """Vérifie que les threads respectent l'ordre logique."""
        result = run(
            ["python3", "/home/chouett/Documents/Projet_avance/Codage-Decodage-main/src/Partie2/dh_genkey.py", "-i", self.input_file, "-o", self.output_file],
            stdout=PIPE,
            stderr=PIPE
        )
        log_content = self.read_log()

        # Vérifie l'ordre logique des threads
        eve_index = log_content.find("Ici Eve")
        alice_index = log_content.find("Ici Alice")
        bob_index = log_content.find("Ici Bob")

        self.assertTrue(alice_index != -1, "Le message d'Alice est manquant.")
        self.assertTrue(eve_index != -1, "Le message d'Eve est manquant.")
        self.assertTrue(bob_index != -1, "Le message de Bob est manquant.")
        self.assertTrue(eve_index < alice_index< bob_index, "L'ordre des threads n'est pas respecté.")
    
    def test_invalid_input_file(self):
        """Tester que le script gère correctement un fichier d'entrée invalide."""
        invalid_input_file = os.path.join(self.temp_dir.name, "invalid_input.txt")

        # Exécuter le script avec un fichier d'entrée inexistant
        result = run(
            ["python3", "/home/chouett/Documents/Projet_avance/Codage-Decodage-main/src/Partie2/dh_genkey.py", "-i", invalid_input_file, "-o", self.output_file],
            stdout=PIPE,
            stderr=PIPE
        )
        log_content = self.read_log()

        # Vérifier que le script échoue avec un code d'erreur approprié et un message clair
        self.assertNotEqual(result.returncode, 0, "Le script aurait dû échouer avec un fichier d'entrée invalide.")
        self.assertIn("Erreur : le fichier d'entrée n'existe pas", result.stderr.decode())

    def test_invalid_input_content(self):      # clear
        """Tester que le script gère correctement un contenu de fichier d'entrée invalide."""
        # Remplir le fichier d'entrée avec un contenu invalide
        with open(self.input_file, "w") as f:
            f.write("Contenu invalide")

        # Exécuter le script avec un contenu de fichier d'entrée invalide
        result = run(
            ["python3", "/home/chouett/Documents/Projet_avance/Codage-Decodage-main/src/Partie2/dh_genkey.py", "-i", self.input_file, "-o", self.output_file],
            stdout=PIPE,
            stderr=PIPE
        )
        log_content = self.read_log()

        # Vérifier que le script échoue avec un code d'erreur approprié
        self.assertNotEqual(result.returncode, 0, "Le script aurait dû échouer avec un contenu d'entrée invalide.")
        self.assertIn("Erreur : le fichier d'entrée n'existe pas.", result.stderr.decode() or result.stdout.decode())
        
if __name__ == "__main__":
    unittest.main()
