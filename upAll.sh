#!/bin/bash

for D in VM*;do
(
	cd $D;sudo vagrant up;	cd ..
)
done


