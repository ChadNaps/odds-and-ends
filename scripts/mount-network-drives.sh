#! /bin/bash

sudo mount -t nfs 192.168.1.5:/media /media/ReadyNAS
sudo mount -t nfs 192.168.1.5:/backup /mnt/ReadyNAS-Backup
# sudo mount /dev/sdb1 /mnt/MyBook-Backup/
# sudo mount -t cifs -o guest,vers=1.0 //192.168.1.6/NASBackup /mnt/MyBookLive-Backup
