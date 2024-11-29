import argparse
import sys
import re
import threading
import queue
import random
import time
import os


TMP_PATH = "tmp/"
LOG_PATH = "logs/"
LOG_FILE = LOG_PATH + "log_part2"

# Couleurs pour l'affichage
RED = "\033[31m"
GREEN = "\033[32m"
BLUE = "\033[34m"
RESET = "\033[0m"

# Événements pour assurer le bon ordre d'exécution des threads
event_alice1 = threading.Event()
event_alice2 = threading.Event()
event_bob1 = threading.Event()
event_bob2 = threading.Event()
event_eve1 = threading.Event()
event_eve2 = threading.Event()
event_fin = threading.Event()


# Fonction utilitaire pour écrire dans le log
def log_message(message):
    """Écrit un message dans un fichier log et l'affiche."""
    with open(LOG_FILE, "a") as f:
        f.write(message + "\n")
    print(message)

def alice(p, g, q):
    # (pour l'affichage)
    presentation = f"{GREEN}Ici Alice{RESET}, "

    # Calcul et envoie de A
    event_alice1.wait()

    a = random.randint(1, p - 1)
    log_message(presentation + "j'ai choisi le nombre secret a = " + str(a) + ".")
    A = pow(g, a, p)
    log_message("J'ai calculé A = " + str(A) + ", je l'envoie à Bob.\n")
    q.put(A)

    event_eve1.set()

    # Réception de B et calcul de la clé
    event_alice2.wait()

    B = q.get()
    log_message(presentation + "j'ai reçu de Bob B = " + str(B) + ".")
    cle = pow(B, a, p)
    log_message("J'ai calculé la clé = " + str(cle) + ".\n")

    event_bob2.set()

    # Conclusion
    event_fin.wait()

    log_message(f" - {GREEN}Alice{RESET} connaît : p = " + str(p) + ", g = " + str(g)
            + ", a = " + str(a) + ", A = " + str(A) + ", B = "+ str(B)
            + " et la clé = " + str(cle) + ".")

    # (Pour l'écrire dans le fichier output, ne fait pas partie de la simulation)
    q.put(cle)

def bob(p, g, q):
    # (pour l'affichage)
    presentation = f"{BLUE}Ici Bob{RESET}, "

    # Calcul et envoie de B
    event_bob1.wait()

    b = random.randint(1, p - 1)
    log_message(presentation + "j'ai choisi le nombre secret b = " + str(b) + ".")
    B = pow(g, b, p)
    log_message("J'ai calculé B = " + str(B) + ", je l'envois à Alice.\n")
    A = q.get() # Réception de A maintenant avant qu'il ne soit remplacé par B dans la queue
    q.put(B)

    event_eve2.set()

    # Réception de A et calcul de la clé
    event_bob2.wait()

    log_message(presentation + "j'ai reçu d'Alice A = " + str(A) + ".")
    cle = pow(A, b, p)
    log_message("J'ai calculé la clé = " + str(cle) + ".\n")

    # Conclusion
    log_message("Au final :")
    event_fin.set()
    log_message(f" - {BLUE}Bob{RESET} connaît : p = " + str(p) + ", g = " + str(g)
            + ", b = " + str(b) + ", B = " + str(B) + ", A = "+ str(A)
            + " et la clé = " + str(cle) + ".")

def eve(p, g, q):
    # (pour l'affichage)
    presentation = f"{RED}Ici Eve{RESET}, "

    log_message(presentation + "je connais p = " + str(p) + " et g = " + str(g) + ".\n")

    event_alice1.set()

    # Interception du message de Alice (A)
    event_eve1.wait()

    A = q.get()
    log_message(presentation + "j'ai intercepté A = " + str(A) + ".\n")
    q.put(A)

    event_bob1.set()

    # Interception du message de Bob (B)
    event_eve2.wait()

    B = q.get()
    log_message(presentation + "j'ai intercepté B = " + str(B) + "\n")
    q.put(B)

    event_alice2.set()

    # Conclusion
    event_fin.wait()
    log_message(f" - {RED}Eve{RESET} connaît : p = " + str(p) + ", g = " + str(g)
            + ", A = " + str(A) + " et B = "+ str(B) + ".")


def simulate_dh_key_exchange(p, g):
    log_message("\nAlice et Bob ont choisi p = " + str(p) + " et g = " + str(g) + ".\n")

    q = queue.Queue()
    thread_alice = threading.Thread(target = alice, args = (p, g, q))
    thread_bob = threading.Thread(target = bob, args = (p, g, q))
    thread_eve = threading.Thread(target = eve, args = (p, g, q))

    # Création des threads
    thread_alice.start()
    thread_bob.start()
    thread_eve.start()

    # Attente de la fin des threads
    thread_alice.join()
    thread_bob.join()
    thread_eve.join()

    # return la clé générée par Alice et Bob
    return q.get()

def get_dh_numbers(input_file_name):
    try:
        with open(TMP_PATH + input_file_name, "r") as input_file:
            input_file_content = input_file.read()
        
        dh_numbers = re.findall(r"\d+", input_file_content)
        
        if len(dh_numbers) != 3:
            raise ValueError("Le contenu du fichier est invalide.")
    
    except FileNotFoundError:
        sys.stderr.write("Erreur : le fichier d'entrée n'existe pas.")
        sys.exit(1)
    except ValueError:
        sys.stderr.write("Erreur : le fichier d'entrée est incorrect.\n")
        sys.exit(2)

    return int(dh_numbers[0]), int(dh_numbers[1])

def write_key(output_file_name, key):
    with open(TMP_PATH + output_file_name, 'w') as output_file:
        output_file.write(str(key))


def main():
    # Crée le dossier tmp s'il n'existe pas
    if not os.path.exists("tmp/"):
        os.makedirs("tmp/")
    
    # Vide le fichier log au début de l'exécution
    with open(LOG_FILE, "w") as log_file:
        log_file.write("")

    # Gestion des arguments
    parser = argparse.ArgumentParser(
                        prog="dh_genkey.py",
                        description="Simule un échange de Diffie-Hellman.")
    parser.add_argument('-i', '--input', type=str, required=True)
    parser.add_argument('-o', '--output', type=str, required=True)
    args = parser.parse_args()

    premier, generateur = get_dh_numbers(args.input)
    key = simulate_dh_key_exchange(premier, generateur)
    write_key(args.output, key)


main()