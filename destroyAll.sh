#!/bin/bash

for D in VM*;do
(
	cd $D
	sudo vagrant destroy
	cd ..
)
done

