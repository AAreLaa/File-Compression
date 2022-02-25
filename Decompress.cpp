#include<iostream>
#include<fstream>
#include<vector>
#include<cstring>
#include<stdlib.h>
using namespace std;

//Creating Huffman Tree Node 
struct Node                                                                                            
{
    unsigned char item;
    unsigned int frequency;
    struct Node* left,* right;
    Node(unsigned char c,Node* l=NULL,Node* r=NULL)
    {
        item=c;
        left=l;
        right=r;
    }
};

//Creating Tree using the compressed file
Node* CreateHuffmanTree(ifstream &input)                                                       
{
    char ch;
    input.get(ch);
    if(ch=='1')
    {
        input.get(ch);
        return (new Node(ch));
    }
    else
    {
        Node* left=CreateHuffmanTree(input);
        Node* right=CreateHuffmanTree(input);
        return(new Node(-1,left,right));
    }
}

//Decoding each binary symbol according to the tree
void decode(ifstream &input,string filename,Node* Root,long long int Total_frequency)             
{
    ofstream output((filename.erase(filename.size()-4)).c_str(),ios::binary);
    if(!output.good())
    {
    	cout<<endl;
        perror("Error");
        exit(-1);
    }
    bool eof=false;
    char bits_8;
    Node* pointer=Root;
    while(input.get(bits_8))
    {
        int counter=7;
        while(counter>=0)
        {
            if(!pointer->left&&!pointer->right)
            {
                output<<pointer->item;
                Total_frequency--;
                if(!Total_frequency)
                {
                    eof=true;
                    break;
                }
                pointer=Root;
                continue;
            }
            if((bits_8&(1<<counter)))
            {
                pointer=pointer->right;
                counter--;
            }
            else
            {
                pointer=pointer->left;
                counter--;
            }
        }
        if(eof)
            break;
    }
    output.close();
}


int main()
{
	//layout
	const int linelength=100; 
	string filename; 
	string header="File Decompression";
	string space((linelength-header.size())/2,' ');  
	string edit(linelength,'=');
	cout<<space<<header;
	cout<<endl<<edit<<endl;
	cout<<endl<<"ENTER NAME OF THE FILE YOU WANT TO DECOMPRESS:\t";
	cin>>filename;
	cout<<endl<<edit<<endl;
	
	//setting input stream
	ifstream infile(filename.c_str(),ios::binary);
	if(!infile.good())                                                                          
    {
    	cout<<endl;
        perror("Error");
        exit(-1);
    }
    
    //Checking valid file to be decompressed
    if(filename.find(".nds")==string::npos)                                                     
    {
    	cout<<endl;
        cout<<"Enter file that needs to be decompressed."<<endl;
        cout<<endl<<edit<<endl;
        exit(-1);
    }
    cout<<endl<<"Decompressing in 3...2...1..."<<endl;
    cout<<endl<<edit<<endl;
    long long int Total_frequency=0;
    char ch;
    
    //Reading Original total frequency from file
    while(infile.get(ch))                                                                  
    {
        if(ch==',')
            break;
        Total_frequency*=10;
        Total_frequency+=ch-'0';
    }
    
    //Remaking Huffman tree from file and decoding
    Node* Huffman_tree= CreateHuffmanTree(infile);                                         
    infile.get(ch);                                                                       
    decode(infile,filename,Huffman_tree,Total_frequency);
    infile.close();
    
    //Deleting the compressed file
    remove(filename.c_str());                                                        
        
    //layout
    cout<<endl<<"File Decompressed Successfully!!!"<<endl;
    cout<<endl<<edit<<endl;
}
