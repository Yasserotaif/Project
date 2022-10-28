/////////////////////////////////////////////////////////////////////LRU REPLACEMENT POLICY/////////////////////////////////////////////////////////////////
int LRU(unsigned long long int *array,int address,int associativity,unsigned long long int tag,int tagbits,int * LRU_array){
    
    int j=0;
    while(LRU_array[j]!=0){                                                                 // Find the least used one to be the victim.
        j=j+1;
    };
    array[j]=1;                                                                             // Replace the victim.
    array[j]=(array[j]<<tagbits)+tag;
    return j;
    
};

void LRU_order(int *array,int associativity,int index){                                 // Function to change the order of LRU array                      
    int temp = array[index];                                                            // store the victim value
    array[index]=associativity - 1;                                                     // Set the recent used block with highest value
    for(int i=0;i<associativity;i++){                                                   
        if( i== index){                                                                 // Change the value in LRU array one by one
            continue;                                                                   // Follow the Rule below.
        }
        else if(array[i]<temp){                                                         // RULE: Value which is smaller than victim value keep the same.                                      
            continue;
            }
        else if(array[i]>temp){                                                         // RULE: Value which is larger than victim value minus one.
            array[i]=array[i]-1;
            }
    };
};


int load_cache(unsigned long long int address,int set_number, int block_size,int associativity,unsigned long long int tag,int tagbits,char replacement_policy, unsigned long long int **array,int miss){
        int set;
        int i;                                                                            //  Load cache & Check HIT/MISS
        unsigned long long int  cache_tag;
        unsigned long long int one =1;                                                
        set = (address/block_size) % set_number;                                          //  Calculate the set which probably contains the data
        bool replacement_required = false;
        for(i=0;i<=associativity;i++){
            if((array[set][i]>>tagbits)==1){                                              // Check Valid-bit
                cache_tag = array[set][i]-(one<<tagbits);                                 // If Valid-bit = 1 , Take the cache_tag from cache. If Valid-bit = 0, says no other data are available. MISS+1, then load/replacement.
                if(cache_tag == tag){                                                     // Check Tag match Cache_tag or not?
                    return miss;                                                          // HIT -> MISS keep the same
                }
                else {
                    if(i==associativity){                                                 // IF check all the data in the set (i=associativity), not a hit. MISS occurs. Then to perform replacement function.
                        replacement_required = true;
                        miss = miss +1;
                        break;
                    }
                }
            }
            else {  
                miss = miss + 1;                                                          // Valid-bit = 0, no need to check left set space. MISS occurs.
                break;
            };
        };
        for(i=0;i<=associativity;i++){                                                   // LOAD THE DATA, If there is empty in the set.
            if (replacement_required){
                Rreplacement(array[set],address,associativity,tag,tagbits);
            }                                                   
            else if (array[set][i]!=0){continue;}                                        // Check the valid-bit, to find which is empty.
            else{
                array[set][i]=array[set][i]+1;                                           
                array[set][i]=array[set][i]<<tagbits;                                    // Set the valid-bit->1 
                array[set][i]=array[set][i]+tag;                                         // Change tag   
                break;
            }
        };
        return miss;
};


int load_cache_LRU(unsigned long long int address,int set_number,int block_size,int associativity,unsigned long long int tag,int tagbits,char replacement_policy, unsigned long long int *array,int* LRU_array,int miss){
        int set;
        int i;
        unsigned long long int  cache_tag;
        unsigned long long int one =1;
        set = (address/block_size) % set_number;
        bool replacement_required = false;                                                    // Mostly similar to the random one
        for(i=0;i<=associativity;i++){
            if(i==associativity){
                        replacement_required = true;
                        miss = miss +1;
                        break;
            }
            else if((array[set][i]>>tagbits)==1){
                cache_tag = array[set][i]-(one<<tagbits);
                if(cache_tag == tag){ 
                    LRU_order(LRU_array[set],associativity,i);                                // IF HIT, Change the order of LRU array.
                    return miss;
                }
                else{continue;}
            }
            else {
                miss = miss + 1;
                break;
            };
        };
        for(i=0;i<=associativity;i++){
            if (replacement_required){
                int index=LRU(array[set],address,associativity,tag,tagbits,LRU_array[set]);   // Perform LRU replacement and keep the victim index.
                LRU_order(LRU_array[set],associativity,index);                                // Change order in LRU array
                break;
            }
            else if (array[set][i]!=0){continue;}
            else{
                array[set][i]=array[set][i]+1;
                array[set][i]=array[set][i]<<tagbits;
                array[set][i]=array[set][i]+tag;
                LRU_order(LRU_array[set],associativity,i);
                break;
            }
        };
        return miss;
};