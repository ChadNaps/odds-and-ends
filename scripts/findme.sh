#! /bin/bash

sudo find / -name "$1" 2>&1 | grep -v "Permission denied"
