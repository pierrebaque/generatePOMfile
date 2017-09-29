#!/bin/bash

rm rectangles.po*
touch rectangles.pom
echo "ROOM 480 270 7 36000" >> rectangles.pom
echo "" >> rectangles.pom
./generate ./images_or/C1.jpg ./extrinsic/extr_CVLab1.xml ./intrinsic/intr_CVLab1.xml 0 1 ./constant.txt >> rectangles.pom
./generate ./images_or/C2.jpg ./extrinsic/extr_CVLab2.xml ./intrinsic/intr_CVLab2.xml 1 1 ./constant.txt >> rectangles.pom
./generate ./images_or/C3.jpg ./extrinsic/extr_CVLab3.xml ./intrinsic/intr_CVLab3.xml 2 1 ./constant.txt >> rectangles.pom
./generate ./images_or/C4.jpg ./extrinsic/extr_CVLab4.xml ./intrinsic/intr_CVLab4.xml 3 1 ./constant.txt >> rectangles.pom
./generate ./images_or/C5.jpg ./extrinsic/extr_IDIAP1.xml ./intrinsic/intr_IDIAP1.xml 4 1 ./constant.txt >> rectangles.pom
./generate ./images_or/C6.jpg ./extrinsic/extr_IDIAP2.xml ./intrinsic/intr_IDIAP2.xml 5 1 ./constant.txt >> rectangles.pom
./generate ./images_or/C7.jpg ./extrinsic/extr_IDIAP3.xml ./intrinsic/intr_IDIAP3.xml 6 1 ./constant.txt >> rectangles.pom
#cat parameters.txt >> rectangles.pom

echo "# The filenames of the view to load" >> rectangles.pom

echo "INPUT_VIEW_FORMAT ../parts/c%c_p6/%f.png " >> rectangles.pom
echo "RESULT_VIEW_FORMAT ./results2/result-f%f-c%c.png" >> rectangles.pom
echo "RESULT_FORMAT ./results2/proba-f%f.dat" >> rectangles.pom
echo "CONVERGENCE_VIEW_FORMAT ./results2/convergence/f%f-c%c-i%i.png" >> rectangles.pom
echo "PROBA_IGNORED 0.9999" >> rectangles.pom
echo "MAX_NB_SOLVER_ITERATIONS 300" >> rectangles.pom
echo "PROCESS 50 6" >> rectangles.pom