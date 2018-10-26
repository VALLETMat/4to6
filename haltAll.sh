#!/bin/bash

for D in VM*;do
	cd $D
	sudo vagrant halt
	cd ..
done

