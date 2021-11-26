#include "test.h"

test::test()
{
    QVector <double> vector_1(100);
    QVector <double> vector_2(100);
    QVector <double> vector_3(100);
    QVector <double> vector_4(100);
    QVector <double> vector_5(100);

    for (int i=0; i<100; ++i) // data for graph 0
       {
         vector_1[i] = i;
         vector_2[i] = i+100;
         vector_3[i] = i+200;
         vector_4[i] = i+300;
         vector_5[i] = i+400;
       }

     vector.push_back(vector_1);
     vector.push_back(vector_2);
     vector.push_back(vector_3);
     vector.push_back(vector_4);
     vector.push_back(vector_5);


}
