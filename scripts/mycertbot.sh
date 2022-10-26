#! /usr/bin/bash

sudo /home/pi/venv/bin/certbot certonly \
	--authenticator dns-infomaniak \
	--dns-infomaniak-credentials /home/pi/venv/secret/credentials.ini \
	--server https://acme-v02.api.letsencrypt.org/directory \
	--agree-tos \
	--rsa-key-size 4096 \
	-d 'chadnaps.com' \
	-d '*.chadnaps.com'
