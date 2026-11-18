sudo apt install -y nginx python3-venv ufw
sudo mkdir -p /opt/portal && sudo cp app.py requirements.txt /opt/portal && cd /opt/portal
sudo python3 -m venv venv && sudo venv/bin/pip install -r requirements.txt
sudo cp portal.service /etc/systemd/system/ && sudo systemctl daemon-reload
sudo systemctl enable --now portal            # start now and at every boot
systemctl status portal                       # expect: active (running)
curl -s http://127.0.0.1:8000/health          # expect: {"status":"ok"}
sudo cp nginx.conf /etc/nginx/conf.d/portal.conf && sudo nginx -t && sudo systemctl reload nginx
curl -si http://localhost/health | head -1    # expect: HTTP/1.1 200 OK (through Nginx)
sudo ufw allow 22/tcp && sudo ufw allow 80/tcp && sudo ufw allow 443/tcp && sudo ufw enable
sudo ufw status                               # only 22, 80 and 443 listed
# recovery test: kill the application and see systemd restart it
sudo pkill -f gunicorn; sleep 3; systemctl is-active portal     # expect: active
