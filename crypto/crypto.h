#include <string>
class HashGenerator
{
public:
    static HashGenerator& get();
    static std::string sha256(const std::string& digest);
private:
    HashGenerator() {
    }

};