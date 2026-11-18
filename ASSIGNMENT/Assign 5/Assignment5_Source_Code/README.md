# CSA0404 - Operating Systems - Assignment 5 (CO5): Linux, Containers and Virtualisation

Name: Mohammed Subahaan H
Register number: 192572165
Course: Operating Systems (CSA0404)
GitHub: https://github.com/Subahaan-cmd/CSA-0404-OPERATING-SYSTEM

Linux: Ubuntu 24.04.5 LTS (kernel 6.18) | bash 5.2 | Python 3.12.3 | gcc 13.3.0 (no C programs in this assignment)

## Folders and results
A-design/  accounts.sh, portal.service, netplan.yaml, nginx.conf, ufw.sh, site.yml (Ansible), hpa.yaml
B1/        logsummary.sh, disk_alert.sh, loganalysis.sh, crontab.txt, deploy.sh and outputs
           Result: summary output equals the expected output (200:4 302:1 403:1 404:1; 10.0.0.5 4; 43200 bytes)
B2/        app.py, requirements.txt, Dockerfile, Dockerfile.multistage, docker-compose.yml, commands.txt
           Result: non-root user portal, port 8000, gunicorn; Compose with a database volume; --cpus/--memory limits
B3/        deployment.yaml (3 replicas, selector, requests/limits), hpa.yaml, bench.sh, kubernetes.txt
B4/        capacity.py (+ output), docker-compose.yml, lms-and-quota.txt
           Result: peak 1,000 requests/s needs 8 containers at 200 requests/s each (10 with spare)

## How to run
bash B1/logsummary.sh
bash B1/disk_alert.sh 80
bash B1/loganalysis.sh B1/sample_access.log
python3 B4/capacity.py
docker build -t result-portal B2/ && docker run -p 8000:8000 result-portal
kubectl apply -f B3/deployment.yaml
