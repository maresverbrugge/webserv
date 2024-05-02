#include <iostream>
#include <string>

int hex_to_int(char hex)
{
    int x;
    if (hex >= '0' && hex <= '9')
        x = hex - '0';
    else if (hex >= 'A' && hex <= 'F')
        x = hex - 'A' + 10;
    else if (hex >= 'a' && hex <= 'f')
        x = hex - 'a' + 10;
    else
        throw (400);
    return (x);
}

std::string decodePercentEncodedString(std::string s)
{
    std::string result;
    for (size_t i = 0; i < s.length(); i++)
    {
        if (s[i] == '%')
        {
            if (i + 2 < s.length())
            {
                result += (16 * hex_to_int(s[i + 1])) + hex_to_int(s[i + 2]);
                i += 2;
            }
            else
                throw (400);
        }
        else
            result += s[i];
    }
    return (result);
}

int main(void)
{
    std::string encoded = "\%67\%69thub";
    std::cout << decodePercentEncodedString(encoded);

}