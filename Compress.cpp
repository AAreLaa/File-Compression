#include<iostream>
#include<fstream>
#include<vector>
#include<cstring>
#include<stdlib.h>
#define SIZE 256   
using namespace std;

//Creating Huffman Tree Node
struct Node                                                                                            
{
    unsigned char item;
    unsigned int frequency;
    struct Node* left,* right;
    Node(unsigned char c,unsigned int f,Node* l=NULL,Node* r=NULL)
    {
    	left=l;
    	right=r;
        item=c;
        frequency=f;
    }
};

//Creating Heapify function
void Minheapify(vector<Node*> &array,int i,int size)                                                     
{
    int smallest=i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if(left<size&&array[left]->frequency<array[smallest]->frequency)
        smallest=left;
    else if(right<size&&array[right]->frequency<array[smallest]->frequency)
        smallest=right;
    if(smallest!=i)
    {
        swap(array[i],array[smallest]);
        Minheapify(array,smallest,size);
    }
}

//Extracting minimum value node from minheap
Node* extractmin(vector<Node*> &array)                                                                    
{
    if(array.size()<1)
        return NULL;
    Node* temp=array[0];
    array[0]=array.back();
    array.pop_back();
    Minheapify(array,0,array.size()-1);
    return temp;
}

//Inserting item in Minheap
void InsertMinHeap(vector<Node*> &array,Node* item)                                                 
{
    array.push_back(item);
    int i=array.size()-1;
    while(i>0&&array[(i-1)/2]->frequency>array[i]->frequency)
    {
        swap(array[i],array[(i-1)/2]);
        i=(i-1)/2;
    }
}

//Creating Minheap
void CreateMinHeap(vector<Node*> &array,int size)
{
    for(int i=(size-1)/2;i>=0;i--)
    {
        Minheapify(array,i,size);
    }
}

//Using Vector,storing code for each item
void StoreCode(Node* Root,char SingleCode[],int index,vector<long long int> &Huffman_codemap)    
{
    if(Root->left)
    {
        SingleCode[index]='0';
        StoreCode(Root->left,SingleCode,index+1,Huffman_codemap);

    }
    if(Root->right)
    {
        SingleCode[index]='1';
        StoreCode(Root->right,SingleCode,index+1,Huffman_codemap);
    }
    if(!Root->left&&!Root->left)
    {
        for(int i=index;i>=0;i--)
        {
          if(i!=index)
          {
            Huffman_codemap[Root->item]*=10;
            Huffman_codemap[Root->item]+=SingleCode[i]-'0';
          }
          else
            Huffman_codemap[Root->item]=1;
        }
    }
}

//Writing tree to file
void StoreTree(ofstream &input,Node* Root)                                                          
{
    if(!Root->left&&!Root->right)
    {
        input<<'1';
        input<<Root->item;
    }
    else
    {
        input<<'0';
        StoreTree(input,Root->left);
        StoreTree(input,Root->right);
    }
}

//Implementing Huffman Algorithm
Node* Huffman(long long int Count[])                                                                
{
    vector<Node*>minheap;
    for(int i=0;i<SIZE;i++)
        if(Count[i]!=0)
            minheap.push_back(new Node(i,Count[i]));
    CreateMinHeap(minheap,minheap.size()-1);
    while(minheap.size()!=1)
    {
        Node* temp=new Node(-1,0,extractmin(minheap),extractmin(minheap));
        temp->frequency=temp->left->frequency+temp->right->frequency;
        InsertMinHeap(minheap,temp);
    }
    return(minheap[0]);
}

//Writing to a new file(Compressed)
void WriteCompressed(ifstream &input,ofstream &output,vector<long long int > &Huffman_codemap)    
{
    char ch;
    unsigned char bits_8;
    long long int counter=0;
    while(input.get(ch))
    {
        long long int temp=Huffman_codemap[static_cast<unsigned char>(ch)];
        while(temp!=1)
        {
          bits_8<<=1;
          if((temp%10)!=0)
                bits_8|=1;
          temp/=10;
          counter++;
          if(counter==8)
            {
                output<<bits_8;
                counter=bits_8=0;
            }
        }
    }
    while(counter!=8)
    {
        bits_8<<=1;
        counter++;
    }
    output<<bits_8;
    output.close();
}

int main()
{
    vector<long long int > Huffman_codemap;                                                    
    Huffman_codemap.resize(SIZE);
    long long int Count[SIZE]={0};
    
    //layout
    const int linelength=100;
    string filename;
    string header="File Compression";
    string space((linelength-header.size())/2,' ');
    string edit(linelength,'=');
    cout<<space<<header;
    cout<<endl<<edit<<endl;
    cout<<endl<<"ENTER NAME OF THE FILE YOU WANT TO COMPRESS:\t";
    cin>>filename;
    cout<<endl<<edit<<endl;
    
    //setting input stream and reading file if it exists
    ifstream infile(filename.c_str(),ios::binary);
    if(!infile.good())                                                                          
    {
    	cout<<endl;
        perror("ERROR");
        exit(-1);
    }
    cout<<endl<<"Compressing in 3...2...1..."<<endl;
    cout<<endl<<edit<<endl;
    char ch;
    while(infile.get(ch))                                                                       
        Count[static_cast<unsigned char>(ch)]++;
    infile.clear();                                                                             
    infile.seekg(0);
    
    //Creating Huffman tree                                                  
	Node* tree=Huffman(Count);
    
    //Giving filename an additional extension nds to represent compressed file                                                          
    ofstream outfile((filename+".nds").c_str(),ios::binary);                                    
    if(!outfile.good())                                                                         
    {
    	cout<<endl;
        perror("ERROR");
        exit(-1);
    }
   
    //Writing to file
    outfile<<tree->frequency;                                                                  
    outfile<<',';
    StoreTree(outfile,tree);
    outfile<<' ';
    char SingleCode[16];
    StoreCode(tree,SingleCode,0,Huffman_codemap);
    WriteCompressed(infile,outfile,Huffman_codemap);                              
    infile.close();                                                                             
    outfile.close();
    
    //Deleting the Original file                                        
    remove(filename.c_str());                                                               
        
    //layout
    cout<<endl<<"File Compressed Successfully!!!"<<endl;
    cout<<endl<<edit<<endl;
}
