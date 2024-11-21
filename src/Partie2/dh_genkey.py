import argparse
import sys
import re
import threading
import queue
import random
import time


# Couleurs pour l'affichage
RED = "\033[31m"
GREEN = "\033[32m"
BLUE = "\033[34m"
RESET = "\033[0m"

# Evenements pour assurer le bon ordre d'execution des threads
event_alice1 = threading.Event()
event_alice2 = threading.Event()
event_bob1 = threading.Event()
event_bob2 = threading.Event()
event_eve1 = threading.Event()
event_eve2 = threading.Event()
event_fin = threading.Event()


def alice(p, g, q):
    # (Pour l'affichage)
    presentation = f"{GREEN}Ici Alice{RESET}, "

    # Calcul et envoie de A
    event_alice1.wait()

    a = random.randint(1, p - 1)
    print(presentation + "j'ai choisi le nombre secret a = " + str(a) + ".")
    A = pow(g, a, p)
    print("J'ai calcule A = " + str(A) + ", je l'envoie à Bob.\n")
    q.put(A)

    event_eve1.set()

    # Reception de B et calcul de la cle
    event_alice2.wait()

    B = q.get()
    print(presentation + "j'ai reçu de Bob B = " + str(B) + ".")
    cle = pow(B, a, p)
    print("J'ai calculé la cle = " + str(cle) + ".\n")

    event_bob2.set()

    # Conclusion
    event_fin.wait()

    print(f" - {GREEN}Alice{RESET} connait : p = " + str(p) + ", g = " + str(g)
            + ", a = " + str(a) + ", A = " + str(A) + ", B = "+ str(B)
            + " et la cle = " + str(cle) + ".")

    # (Pour l'ecrire dans le fichier output, ne fait pas partie de la simulation)
    q.put(cle)

def bob(p, g, q):
    # (Pour l'affichage)
    presentation = f"{BLUE}Ici Bob{RESET}, "

    # Calcul et envoie de B
    event_bob1.wait()

    b = random.randint(1, p - 1)
    print(presentation + "j'ai choisi le nombre secret b = " + str(b) + ".")
    B = pow(g, b, p)
    print("J'ai calcule B = " + str(B) + ", je l'envois à Alice.\n")
    A = q.get() # Reception de A maintenant avant qu'il ne soit remplace par B dans la queue
    q.put(B)

    event_eve2.set()

    # Reception de A et calcul de la cle
    event_bob2.wait()

    print(presentation + "j'ai reçu d'Alice A = " + str(A) + ".")
    cle = pow(A, b, p)
    print("J'ai calcule la cle = " + str(cle) + ".\n")

    # Conclusion
    print("Au final :")
    event_fin.set()
    print(f" - {BLUE}Bob{RESET} connait : p = " + str(p) + ", g = " + str(g)
            + ", b = " + str(b) + ", B = " + str(B) + ", A = "+ str(A)
            + " et la cle = " + str(cle) + ".")

def eve(p, g, q):
    # (Pour l'affichage)
    presentation = f"{RED}Ici Eve{RESET}, "

    print(presentation + "je connais p = " + str(p) + " et g = " + str(g) + ".\n")

    event_alice1.set()

    # Interception du message de Alice (A)
    event_eve1.wait()

    A = q.get()
    print(presentation + "j'ai intercepte A = " + str(A) + ".\n")
    q.put(A)

    event_bob1.set()

    # Interception du message de Bob (B)
    event_eve2.wait()

    B = q.get()
    print(presentation + "j'ai intercepte B = " + str(B) + "\n")
    q.put(B)

    event_alice2.set()

    # Conclusion
    event_fin.wait()
    print(f" - {RED}Eve{RESET} connait : p = " + str(p) + ", g = " + str(g)
            + ", A = " + str(A) + " et B = "+ str(B) + ".")


def simulate_dh_key_exchange(p, g):
    print("\nAlice et Bob ont choisi p = " + str(p) + " et g = " + str(g) + ".\n")

    q = queue.Queue()
    thread_alice = threading.Thread(target = alice, args = (p, g, q))
    thread_bob = threading.Thread(target = bob, args = (p, g, q))
    thread_eve = threading.Thread(target = eve, args = (p, g, q))

    # Creation des threads
    thread_alice.start()
    thread_bob.start()
    thread_eve.start()

    # Attente de la fin des threads
    thread_alice.join()
    thread_bob.join()
    thread_eve.join()

    # return la cle generee par Alice et Bob
    return q.get()

def get_dh_numbers(input_file_name):
    try:
        with open("tmp/" + input_file_name, "r") as input_file:
            input_file_content = input_file.read()
        
        dh_numbers = re.findall(r"\d+", input_file_content)
    except FileNotFoundError:
        print("Erreur : le fichier d'entrée n'existe pas.")
        sys.exit(1)

    if len(dh_numbers) != 3:
        print("Erreur : le fichier d'entrée est incorrect.")
        sys.exit(2)
    
    return int(dh_numbers[0]), int(dh_numbers[1])

def write_key(output_file_name, key):
    with open("tmp/" + output_file_name, 'w') as output_file:
        output_file.write(str(key))


def main():
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