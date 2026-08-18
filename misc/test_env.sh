#!/bin/bash
# #! = shebang : permet d'éxecuter un script .sh sans directement via son chemin
touch sortie_test.txt                                           # création du fichier de sortie
socat -lf sortie_test.txt -d -d pty,raw,echo=0 pty,raw,echo=0 & # Execution de la commande en background avec &

echo $! >socat.pid # $! Contient le PID de la derniere commande lancée en background
Time=0

while (($(wc -l <sortie_test.txt) < 2)); do # Tant que le nombre de ligne dans sortie_test.txt n'est pas plus grand que 2 sleep
  if (($Time == 6)); then
    # kill -0 permet de voir les permissions du process donc s'il n'existe pas la sortie sera NULL
    if kill -0 "$(cat socat.pid)" 2>/dev/null; then # 2> /dev/null permet de ne pas afficher le resultat de la commande dans le terminal
      kill -15 "$(cat socat.pid)"
    fi
    rm sortie_test.txt socat.pid
    echo "Error : socat not finished under 3 seconds." >&2 #Redirige vers stderr
    exit 1
  fi
  sleep 0.5
  ((Time++))
done

awk 'NR<=2 {print $NF}' sortie_test.txt >socat_paths.txt #NR = Row Number et NF = Last Field
rm sortie_test.txt
