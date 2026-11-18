# create the service account without login and without a home directory
sudo useradd --system --no-create-home --shell /usr/sbin/nologin portal
sudo groupadd auditors && sudo usermod -aG auditors alice
# allow the deploy account to restart only the portal service
echo 'deploy ALL=(root) NOPASSWD: /bin/systemctl restart portal, /bin/systemctl status portal' | sudo tee /etc/sudoers.d/deploy
sudo chmod 0440 /etc/sudoers.d/deploy
