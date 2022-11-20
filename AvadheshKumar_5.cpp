#include<iostream>
#include<cmath>
#include<string>
#include<fstream>
using namespace std;

int index=0;
char characters[94];
char ch[94];
int size_of_input, size_of_huff;

struct item{
    char value;
    int frequency;
};

struct MinHeapNode{
    struct item value;
    struct MinHeapNode *left, *right;
};

struct MinHeap{
    int size;
    int capacity;
    struct MinHeapNode **arr;
};

struct MinHeapNode *newNode(char item, unsigned freq) {
  struct MinHeapNode *temp = (struct MinHeapNode *)malloc(sizeof(struct MinHeapNode));

  temp->left = temp->right = NULL;
  temp->value.value = item;
  temp->value.frequency = freq;

  return temp;
}

struct MinHeap * createMinHeap(int capacity){
    struct MinHeap *min = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    min->size = 0;
    min->capacity = capacity;
    min->arr = (struct MinHeapNode **)malloc(min->capacity * sizeof(struct MinHeapNode *));
    return min;
}

void exchange(struct MinHeapNode **a, struct MinHeapNode **b){
    struct MinHeapNode  *temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(struct MinHeap *min, int index){ 
    int l = 2*index + 1;
    int r = 2*index + 2;
    int smallest = index;

    if((l < min->size) && (min->arr[l]->value.frequency < min->arr[smallest]->value.frequency)){
        smallest = l;
    }
    if((r < min->size) && (min->arr[r]->value.frequency < min->arr[smallest]->value.frequency)){
        smallest = r;
    }

    if (smallest != index) {
        exchange(&min->arr[smallest],&min->arr[index]);
        heapify(min, smallest);
    }
}

struct MinHeapNode * DeleteMinNode(struct MinHeap *min){
    struct MinHeapNode *temp = min->arr[0];
    min->arr[0] = min->arr[min->size - 1];
    --min->size;
    heapify(min, 0);
    return temp;
}

void insertMinHeap(struct MinHeap *min, struct MinHeapNode *node ){
    ++min->size;
    int i = min->size - 1;
    while(i && node->value.frequency < min->arr[(i-1)/2]->value.frequency){
        min->arr[i] = min->arr[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    min->arr[i] = node;
}

void build_Min_Heap(struct MinHeap *min){
    int n = min->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        heapify(min, i);
}

struct MinHeap * Heap(item arr[], int size){
    struct MinHeap *min = createMinHeap(size);

    for(int i = 0; i < size; ++i)
        min->arr[i] = newNode(arr[i].value, arr[i].frequency);

    min->size = size;
    build_Min_Heap(min);

    return min;
}

void StoreCodes(struct MinHeapNode *root, int code[], string arr[], int top){
    if(root->left){
        code[top] = 0;
        StoreCodes(root->left, code, arr, top + 1);
    }

    if(root->right){
        code[top] = 1;
        StoreCodes(root->right, code, arr, top + 1);
    }
    if(!(root->left) && !(root->right)){
        characters[index]=root->value.value;
        for (int i = 0; i < top; i++){
            if(i==0)
                arr[index] = to_string(code[i]);
            arr[index] += to_string(code[i]);
        }
        index++;
    }
}

void Huffman_Coding(item arr[], int size, string codes[]){
    struct MinHeapNode *l, *r, *t;
    struct MinHeap *min = Heap(arr, size);

    while(!(min->size == 1)){
        l = DeleteMinNode(min);
        r = DeleteMinNode(min);

        t = newNode('!', (l->value.frequency + r->value.frequency));

        t->left = l;
        t->right = r;

        insertMinHeap(min, t);
    }
    int code[20];
    index=0;
    StoreCodes(t, code, codes, 0);
}

int Scan(item arr[], string *input){
    string file;
    FILE* myfile;
    cout<<endl<<"Enter file name to scan: ";
    cin>>file;
    myfile = fopen(file.c_str(), "r");
    if(myfile == NULL){
        cout<< "file cannot be opened!";
    }   
    else{
        while(!feof(myfile)){
            char ch;
            ch = fgetc(myfile);
            (*input) += ch;
        }
    }
    fseek(myfile, 0, SEEK_END);
    size_of_input = ftell(myfile);
    fclose(myfile);
    int x = 0, y = 0, size = (*input).length();
    for (int i = 0; i < 94; i++)
        arr[i].frequency = 0;
    
    for (char c = ' '; c <= '~'; c++){
        y = 0;
        for (int i = 0; i < size; i++){
            if((*input)[i]==c){
                if(arr[x].frequency==0){
                    y = 1;
                    arr[x].value = c;
                    arr[x].frequency++;
                }
                else{
                    arr[x].frequency++;
                }
            }
        }
        if(y==1)
            x++;
    }
    return x;
}

void merge(string codes[], string input, string& output, int n){
    char c;
    for (int j = 0; j < input.length(); j++){
        c = input[j];
        for (int i = 0; i < n; i++){
            if (c == characters[i]){
                if(j==0)
                    output = codes[i];
                else{
                    output += codes[i];
                }            
            }
        }
    }
}

void spilt(string input, string& output, int n){
    char c;
    int x;
    int out[7];
    for (int j = 0; j < input.length() - 1; j++){
        c = input[j];
        x = c;
        int i = 0;
        while (x > 0) {
            out[6-i] = x % 2;
            x = x / 2;
            i++;
        }
        for(; i < 7 ; i++)
            out[6-i]=0;
        for(int k = 0; k < 7 && j != (input.length()-2); k++){
            char a = out[k]+48;
            output += a;
        }   
        if(j==input.length()-2){
            for(int k=0; k < (n%7); k++){
                char a=out[k] + 48;
                output += a;
            }
        }
    }
}

void string_to_binary(string arr, string& output){
    int out[8];
    out[0]=0;
    char c;
    int o;
    for(int j=0; j<arr.length(); j+=7){
        for(int i=0; i<7; i++){
            if((i+j)>=arr.length()){
                out[i]=0;
            }
            else{
            c = arr[j+i];
            o=c-48;
            out[i+1]=o;
            }
        }
        int x=0;
        for(int i=0; i<8; i++)
            x=x+(out[7-i]*pow(2,i));
    
        char y = x;
        if(j==0)
        output = y;
        else{
            output += y;
        }
    }
}

void binary_to_string(string codes[], string input, string& output, int n){
    string temp;
    char c;
    for(int i=0; i < input.length(); i++){
        c=input[i];
        temp += c;
        for(int j=0; j < n; j++){
            if(temp==codes[j]){
                output+=ch[j];
                temp = "\0";
            }
        }
    }
}

void Encode(string codes[],string input, int n){
    string print, output;
    
    merge(codes, input, output, n);
    string_to_binary(output, print);
    
    ofstream huff;
    huff.open("output.huff");
    
    huff<<n<<" ";
    huff<<output.length()<<" ";    
    for (int i = 0; i < n; i++)
        huff<<characters[i];
    huff<<" ";
    for (int i = 0; i < n; i++)
        huff<<codes[i]<<" ";
    huff<< print;
    huff.close();
}

void stactics(){
    cout<<endl<<"size_of_input"<<" "<<size_of_input<<endl<<"size_of_huff"<<" "<<size_of_huff<<endl;
    float per;
    per = 1.00-((float)size_of_huff/(float)size_of_input);
    cout<<endl<<"Percentage compression: "<<per*10<<endl;
}

void Decode(){
    string file, input, *codes, output, print;
    int no_of_characters, size_of_orginal_file; 
    
    FILE* myfile;
    cout<<endl<<"Enter file name to decode: ";
    cin>>file;
    myfile = fopen(file.c_str(), "r");
    int i=0, j=0;
    if(myfile == NULL){
        cout<< "file cannot be opened!";
    }   
    else{
        while(!feof(myfile)){
            char c;
            
            if(i==0){
                fscanf(myfile, "%d", &no_of_characters);
                codes = new string[no_of_characters];
            }
            else if(i==1){
                fscanf(myfile, "%d", &size_of_orginal_file);
                cin.clear();
            }
            else if( i==2 ){
                j=0;
            }
            else{
                c = fgetc(myfile);
                if(i<=no_of_characters+3){
                    ch[i-4] = c;
                }
                else if(j <= no_of_characters){
                    if(c == ' '){
                        j++;
                        continue;
                    }
                    else
                    codes[j-1] += c;
                }
                else{
                    input += c;
                }
            }
            i++;
        }
    }
    fseek(myfile, 0, SEEK_END);
    size_of_huff = ftell(myfile);
    fclose(myfile);
    
    for(i=0; i< no_of_characters; i++)
        cout<<ch[i]<<" --> "<<codes[i]<<endl;
    spilt(input, output, size_of_orginal_file);
    binary_to_string(codes, output, print, no_of_characters);
}

int main(){
    item arr[94];
    string input;
    int size = Scan(arr, &input);
    for (int i = 0; i < size; i++)
        cout << arr[i].value << " --> " << arr[i].frequency << endl;

    string code[size];
    Huffman_Coding(arr, size, code);
    for (int i = 0; i < size; i++)
        cout <<characters[i]<<" --> " <<code[i] << endl;
    Encode(code, input, size);
    Decode();
    stactics();
    return 0;
}