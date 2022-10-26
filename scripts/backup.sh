#! /bin/bash
sudo rsync -aAXv / –-delete --exclude={/dev/*,/proc/*,/sys/*,/tmp/*,/run/*,/mnt/*,/media/*,/cdrom/*,/lost+found} /mnt/ReadyNAS/Chad/Backups/server/staging &
