
#include <Common/Framework/Cpp/FrameworkExport.h>

std::string str;

double calc( double x, char action, double y )
{
    std::stringstream tmp;

    tmp << x << action << y;

    str += tmp.str() +  " ";

    switch (action) 
    {
    case '+' :        return x + y;
    case '-' :        return x - y;
    case '*' :        return x * y;
    case '/' :        return x / y;
    }
    return x + y;

}


#define NUM 4
typedef std::vector<double> DoubleList;

struct Partition
{
    DoubleList mGroup1;
    DoubleList mGroup2;

};
typedef std::vector<DoubleList> PartitionList;


char oprand[] = { '+', '-' , '/' ,'*'};
int numbers[] = { 1, 5, 6 ,7 };

DoubleList calc( DoubleList aX, DoubleList aY )
{
    DoubleList dList;

    for( int iAction = 0 ; iAction < 4 ; iAction++ )
        for( int xi = 0 ; xi < aX.size() ; xi++ )
            for( int yi = 0 ; yi < aY.size() ; yi++ )
            {
                dList.push_back( calc( aX[xi], oprand[iAction], aY[yi] ) );
            }

    return dList;
}

void print( PartitionList aPartitionList )
{
    cout << "\r\n";

    for( int i = 0 ; i < aPartitionList.size() ; i++ )
    {
        for( int j = 0 ; j < aPartitionList[i].size() ; j++ )
        {
            cout << aPartitionList[i][j] << ',' ;
        }
        cout << "\r\n";
    }

    cout << "\r\n";
}

PartitionList multiple( DoubleList group ,DoubleList aNum )
{
    PartitionList newPartitionList;
    
    for( int j = 0 ; j < aNum.size() ; j++ )
    {
        DoubleList tempGroup = group;

        tempGroup.push_back( aNum[j] );

        newPartitionList.push_back( tempGroup );
    }

    return newPartitionList;
}

DoubleList decrease( DoubleList x ,DoubleList y )
{
    DoubleList result;

    for( int i = 0 ; i < x.size() ; i++ )
    {
        int j;
        for( j = 0 ; j < y.size() ; j++ )
        {
            if( x[i] == y[j] )
            {
                break;
            }
        }
        if( j == y.size() )
        {
            result.push_back( x[i] );
        }
    }

    assert( result.size() == x.size() - y.size() );

    return result;
}

void deleteRecords( PartitionList& aPartitionList )
{
    PartitionList newPartitionList;
    
    typedef std::set<double> DoubleSet;

    for( int i = 0 ; i < aPartitionList.size() ; i++ )
    {
        DoubleSet doubleSet;

        for( int j = 0 ; j < aPartitionList[i].size() ; j++ )
        {
            doubleSet.insert( aPartitionList[i][j] );
        }

        if( aPartitionList[i].size() == doubleSet.size() )
        {
            newPartitionList.push_back( aPartitionList[i] );

        }
    }

    aPartitionList = newPartitionList;

}

void getPartitions( 
   PartitionList& aPartitionList, 
   DoubleList aNum, 
   int aPartitionCount )
{
    if( aPartitionCount == 0 )
    {
        return;
    }

    PartitionList newPartitionList;

    if( aPartitionList.size() == 0 )
    {
        PartitionList multiplePartitionList = multiple( DoubleList() ,aNum );

        for( int j = 0 ; j < multiplePartitionList.size() ; j++ )
        {
            newPartitionList.push_back( multiplePartitionList[j] );
        }

    }

    for( int i = 0 ; i < aPartitionList.size() ; i++ )
    {
        PartitionList multiplePartitionList = multiple( aPartitionList[i] ,aNum );

        for( int j = 0 ; j < multiplePartitionList.size() ; j++ )
        {
            newPartitionList.push_back( multiplePartitionList[j] );
        }
    }


    aPartitionList = newPartitionList;

    getPartitions( aPartitionList, aNum, aPartitionCount - 1 );

    deleteRecords( aPartitionList );

}

DoubleList calcPermutation( DoubleList aNum )
{
    if( aNum.size() == 1 )
    {
        return aNum;
    }

    DoubleList gList;

    for( int iPart = 1 ; iPart < aNum.size() ; iPart++ )
    {
        PartitionList xPart;
        
        getPartitions( xPart, aNum, iPart );

        for( int i = 0 ; i < xPart.size() ; i++ )
        {
            PartitionList yPart;
            
            getPartitions( yPart, decrease( aNum, xPart[i] ), aNum.size() - iPart );        

            for( int j = 0 ; j < yPart.size() ; j++ )
            {
                DoubleList cList = calc( calcPermutation( xPart[i] ), calcPermutation( yPart[j] ) );

                for( int k = 0 ; k < cList.size() ; k++ )
                {
                    gList.push_back( cList[k] );
                }
            }
        }

    }
    return gList;
}

void run2()
{
    DoubleList num;

    num.push_back(1);
    num.push_back(8);
    num.push_back(6);
    num.push_back(7);
    
//    for( int i = 0 ; i < 100 ; i++ )
//    {
//
//    std::next_permutation(num.begin(), num.end() );
//    std::cout << "After next_permutation: ";
//    std::copy(num.begin(), num.end(), std::ostream_iterator<double>(std::cout, " "));
//    std::cout << std::endl;
//}
//    return;
    
    num = calcPermutation( num );

    for( int i = 0 ; i < num.size() ; i++ )
    {
        if( num[i] == 21 )
        {
            cout << 21 << "\t|\t"<< str << std::endl; 
        }
    }
}

void run1()
{
    char oprand[] = { '+', '-' , '/' ,'*'};

    for( int iAction = 0 ; iAction < 4 ; iAction++ )
        for( int jAction = 0 ; jAction < 4 ; jAction++ )
            for( int kAction = 0 ; kAction < 4 ; kAction++ )
                for( int i = 0 ; i < 4 ; i++ )
                    for( int j = 0 ; j < 3 ; j++ )
                        for( int k = 0 ; k < 2 ; k++ )
                            for( int l = 0 ; l < 1 ; l++ )
                            {

                                char action1 = oprand[iAction];
                                char action2 = oprand[jAction];
                                char action3 = oprand[kAction];

                                std::vector<int> numbers;

                                numbers.push_back(1);  numbers.push_back(5); numbers.push_back(6); numbers.push_back(7);

                                double x1 = numbers[i]; numbers.erase( numbers.begin( ) + i );
                                double x2 = numbers[j]; numbers.erase( numbers.begin( ) + j );
                                double x3 = numbers[k]; numbers.erase( numbers.begin( ) + k );
                                double x4 = numbers[l]; numbers.erase( numbers.begin( ) + l );

                                str = "";

                                if( calc ( calc( calc( x1, action1, x2 ), action2, x3 ), action3, x4 )== 21 )
                                    cout << x1 << action1 << x2 << action2 << x3 << action3 << x4 << "\t|\t"<< str << std::endl; 
                                str = "";

                                if( calc( x1, action1, calc( calc( x2, action2,  x3 ), action3,  x4 ) )== 21 )
                                    cout << x1 << action1 << x2 << action2 << x3 << action3 << x4 << "\t|\t"<< str << std::endl; 
                                str = "";

                                if( calc( x1, action1, calc( calc( x2, action2, x3 ), action3, x4 ) )== 21 )
                                    cout << x1 << action1 << x2 << action2 << x3 << action3 << x4 << "\t|\t"<< str << std::endl; 
                                str = "";

                                if( calc( x1, action1, calc( x2, action2, calc( x3, action3, x4 ) ) )== 21 )
                                    cout << x1 << action1 << x2 << action2 << x3 << action3 << x4 << "\t|\t"<< str << std::endl; 
                                str = "";

                                if( calc( calc( x1, action1, x2 ), action2 , calc( x3 ,action3, x4 ) )== 21 )
                                    cout << x1 << action1 << x2 << action2 << x3 << action3 << x4 << "\t|\t"<< str << std::endl; 
                            }
}

