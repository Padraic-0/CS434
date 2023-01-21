#! /bin/sh
ls -la $2 | tr -s " " | cut -d' ' -f 5,9 | sort -nr