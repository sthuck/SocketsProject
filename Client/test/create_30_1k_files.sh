#!/bin/sh
for i in {1..30}; do
	dd if=/dev/urandom of=small$i.db bs=1k count=1 2> /dev/null
done
