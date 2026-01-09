#include<bits/stdc++.h>
using namespace std;

int gcd(int a, int b)
{
    while(b!=0)
    {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int modInv(int key)
{
    for(int i=1;i<26;i++)
    {
        if((key*i)%26==1)
        {
            return i;
        }
    }
    return -1;
}




void decrypt(char cipher[],int key, char plain[])
{
    int inv  = modInv(key);
    if(inv==-1)
    {
        cout<<"Invalid Key"<<endl;
        return;
    }

    int len = strlen(cipher);
    for(int i=0;i<len;i++)
    {
        if(isalpha(cipher[i]))
        {
            int p = tolower(cipher[i]) - 'a';
            int c = (p*inv)%26;
            plain[i] = char(c + 'a');
        }
        else{
            plain[i] = cipher[i];
        }
    }
    plain[len] = '\0';
}


void frequent(char text[],char result[],int n)
{
    int freq[26]={0};
    for(int i=0;text[i];i++)
    {
        if(isalpha(text[i]))
        {
            freq[tolower(text[i]) - 'a']++;
        }
    }

    for(int i=0;i<n;i++)
    {
        int maxIdx = -1;
        for(int j=0;j<26;j++)
        {
            if(freq[j]>0 && (maxIdx == -1 || freq[j]>freq[maxIdx]))
            {
                maxIdx = j;
            }

        }
        if(maxIdx==-1)break;
        result[i] = char(maxIdx + 'a');
        freq[maxIdx] = 0;
    }
    
}


void attack(char cipher[], char sample_text[]){

    char cw[20][10] = {
        "the","be","to","of","and","a","in","that","have","i",
        "it","for","not","on","with","he","as","you","do","at"
    };

    char top_cipher[3] = {0};
    char top_sample[3] = {0};

    frequent(cipher,top_cipher,3);
    frequent(sample_text,top_sample,3);

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(!top_cipher[i] || !top_sample[j])continue;

            int p = top_sample[j] - 'a';
            int c = top_cipher[i] - 'a';

            if(p==0)continue;


            int found_k = -1;
            for(int k=1;k<26;k++)
            {
                if(gcd(k,26)==1 && (p*k)%26==c)
                {
                    found_k = k;
                    break;
                }
            }

            if(found_k!=-1){
                char decrypted[1000];
                decrypt(cipher,found_k,decrypted);
                int match = 0;
                for(int w = 0;w<20;w++)
                {
                    if(strstr(decrypted,cw[w])!=NULL)
                    {
                        match++;
                    }
                }
                if(match>0)
                {
                    cout<<top_cipher[i]<<endl<<top_sample[j]<<endl;
                    cout<<found_k<<endl;
                    cout<<modInv(found_k)<<endl;
                    cout<<decrypted<<endl;
                    return;
                }
            }

        }
    }

}





int main()
{
    char cipher[1000];
    char sample_text[1000];
    

    cin.getline(cipher,1000);
    
    cin.getline(sample_text,1000);
    
    attack(cipher,sample_text);
    return 0;



}