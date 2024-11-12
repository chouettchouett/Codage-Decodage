import argparse
import sys
import re


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
    
    return int(dh_numbers[0]), int(dh_numbers[1]), int(dh_numbers[2])

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

    premier, generateur, ordre = get_dh_numbers(args.input)
    
    key = 1 # TEMPORAIRE

    write_key(args.output, key)


main()