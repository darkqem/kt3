#include <iostream>
#include <string>
#include <curl/curl.h>



using namespace std;
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(void)
{
    CURL* curl;
    CURLcode res;
    string readBuffer;
    string subreadBuffertag;
    string url;
    int id[151];
    int mdaaa[151];
    int pos = 0;
    char month1[101]{};
    char month2[101]{};
    char year1[101]{};
    char year2[101]{};
    curl = curl_easy_init();
    if (curl) 
    {
        url = "https://dummyjson.com/posts?limit=150";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        for (int i = 1; i < 151; i++)
        {
             if(readBuffer.find("userId") != string::npos)
             {
                 pos = readBuffer.find("userId")+8;
                 
                 if(readBuffer[pos+1]==',')
                 { 
                     char a = readBuffer[pos];
                     mdaaa[i] = a - '0';
                 }
                 else
                 { 
                     string rez{ readBuffer[pos] ,readBuffer[pos+1] };
                     mdaaa[i] = stoi(rez);
                 }
                 if (readBuffer.find("tags") != string::npos)
                 {
                     pos = readBuffer.find("tags") + 6;
                     subreadBuffertag = readBuffer.substr(pos, 36);
                     pos = subreadBuffertag.find("]")+1;
                     subreadBuffertag.replace(pos, 21, " ");
                 }
                 if ((subreadBuffertag.find("love") != string::npos) or (subreadBuffertag.find("history") != string::npos) or (subreadBuffertag.find("fiction") != string::npos) or (subreadBuffertag.find("english") != string::npos))
                 {
                     id[i] = mdaaa[i]; 
                 }
                 pos = readBuffer.find("reactions") + 12;
                 readBuffer.replace(0, pos, " ");
             }
        }
        int N,n;
        n = 151;
        N = 151;
        for (int i = 1; i < 151; i++)
        {
            if (id[i] == -858993460)
                id[i] = 0;
        }
        for (int i = 1; i < N - 1; i++)
            for (int j = i + 1; j < N; j++)
                if (id[i] == id[j])
                {
                    for (int k = j; k < N; k++)
                        id[k] = id[k + 1];
                    N--;
                    j--;
                }

        for (int i = 1; i < n; i++)
        {
            if (i >= N) id[i] = 0;
        }
        string city;
        string street;
        string name;
        string surname;
        for (int i = 1; i < 101; i++)
        {
            url = "https://dummyjson.com/users/" + to_string(id[i]);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            if (readBuffer.find("female") != string::npos) 
            {
                readBuffer.clear();
                url.clear(); 
            }
            else
            {
                if (readBuffer.find("cardExpire") != string::npos)
                {
                    pos = readBuffer.find("cardExpire") + 13;
                    month1[i] = (readBuffer[pos]);
                    month2[i] = (readBuffer[pos + 1]);
                    year1[i] = (readBuffer[pos + 3]);
                    year2[i] = (readBuffer[pos + 4]);
                    if (year2[i] == '3')
                    {
                        if (readBuffer.find(",\"address\":{\"address\":\"") != string::npos)
                        {
                            pos = readBuffer.find(",\"address\":{\"address\":\"") + 22;
                            city = readBuffer.substr(pos, 50);
                            pos = city.find(",") + 1;
                            city.replace(pos, 21, " ");
                            int pose = city.find(",");
                            city.replace(pose, 21, "");
                            if (readBuffer.find("\",\"city\":\"") != string::npos)
                            {
                                pos = readBuffer.find("\",\"city\":\"") + 9;
                                street = readBuffer.substr(pos, 50);
                                pos = street.find(",") + 1;
                                int pose = street.find(",");
                                street.replace(pose, 44, "");
                                if (readBuffer.find("firstName") != string::npos)
                                {
                                    pos = readBuffer.find("firstName") + 12;
                                    name = readBuffer.substr(pos, 10);
                                    pos = name.find(",") + 1;
                                    int pose = name.find("\"");
                                    name.replace(pose, 10, "");
                                }
                                else
                                {
                                    readBuffer.clear();
                                    url.clear();
                                }
                                if (readBuffer.find("lastName") != string::npos)
                                {
                                    pos = readBuffer.find("lastName") + 11;
                                    surname = readBuffer.substr(pos, 10);
                                    pos = surname.find(",") + 1;
                                    int pose = surname.find("\"");
                                    surname.replace(pose, 10, "");
                                }
                                else
                                {
                                    readBuffer.clear();
                                    url.clear();
                                }
                                
                            }
                            else
                            {
                                readBuffer.clear();
                                url.clear();
                            }

                            
                            cout << id[i] << " - " << month1[i] << month2[i] << "/" << year1[i] << year2[i] << "  Street:" << city << "  City:" << street << "    name:" << name <<" , "<<surname<< endl;
                        }
                        else
                        {
                            readBuffer.clear();
                            url.clear();
                        }
                    }
                    else
                    {
                        readBuffer.clear();
                        url.clear();
                    }

                }
                else
                {
                    readBuffer.clear();
                    url.clear();
                }
            }
            readBuffer.clear();
            url.clear();
        }
        readBuffer.clear();
        url.clear();
        curl_easy_cleanup(curl);
    }
    return 0;
} 

