sudo ufw default deny incoming
sudo ufw default allow outgoing
sudo ufw allow from 10.20.3.0/24 to any port 22 proto tcp    # SSH only from management
sudo ufw allow 80/tcp                                         # HTTP (redirects to HTTPS)
sudo ufw allow 443/tcp                                        # HTTPS
sudo ufw enable
sudo ufw status verbose

# /etc/ssh/sshd_config (changed lines)
PermitRootLogin no
PasswordAuthentication no
PubkeyAuthentication yes
AllowUsers alice bob deploy
