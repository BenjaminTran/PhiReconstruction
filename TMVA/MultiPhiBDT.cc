#include "PhiBDT.cc"

void MultiPhiBDT()
{
    //std::vector<double> pt = {0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6};
    //std::vector<double> pt = {0,0.5,1,1.5,2,2.5,3,3.5,4}; // For use at low pt
    std::vector<double> pt = {1,1.5,2,2.5,3,3.5,4}; // For use at dau pt > 0.5

    //std::vector<int> sigTrainTest = { 0, 60000 , 60000 , 60000 , 60000 , 60000 , 60000 , 0 , 0 , 0 , 0 , 0};
    // For dau pt > 0.5
    std::vector<int> sigTrainTest = { 60000 , 60000 , 60000 , 60000 , 60000 , 0 };
    // For MC bkg
    //std::vector<int> bkgTrainTest = {80000 , 80000 , 80000 , 80000 , 80000 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
    // For data bkg
    //std::vector<int> bkgTrainTest = {80000 , 80000 , 80000 , 80000 , 80000 , 80000 , 80000 , 80000 , 80000 , 80000 , 0 , 0};
    std::vector<int> bkgTrainTest = {80000 , 80000 , 80000 , 80000 , 80000 , 0}; // For dau pt > 0.5

    for(int i=0; i<pt.size()-1; i++)
    {
        PhiBDT(pt[i],pt[i+1],sigTrainTest[i],bkgTrainTest[i],false);
    }
}
