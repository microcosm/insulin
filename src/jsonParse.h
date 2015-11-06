#include "ofxJSON.h"

class jsonParse : public ofThread {

    string url = "https://andycgm.azurewebsites.net/api/v1/entries/sgv.json?find[dateString][$gte]=2015-08-28";
    ofxJSONElement response;
    Json::Value root;

    void threadedFunction() {
        while(isThreadRunning()) {
            if (response.open(url)) {
                root = (Json::Value) response;
                printValueTree(root);
            } else {
                ofLogNotice("ofApp::setup") << "Failed to parse JSON";
            }
            //lock();
            
            //unlock();
        }
    }
    
    std::string normalizeFloatingPointStr(double value) {
        char buffer[32];
#if defined(_MSC_VER) && defined(__STDC_SECURE_LIB__)
        sprintf_s(buffer, sizeof(buffer), "%.16g", value);
#else
        snprintf(buffer, sizeof(buffer), "%.16g", value);
#endif
        buffer[sizeof(buffer) - 1] = 0;
        std::string s(buffer);
        std::string::size_type index = s.find_last_of("eE");
        if (index != std::string::npos) {
            std::string::size_type hasSign =
            (s[index + 1] == '+' || s[index + 1] == '-') ? 1 : 0;
            std::string::size_type exponentStartIndex = index + 1 + hasSign;
            std::string normalized = s.substr(0, exponentStartIndex);
            std::string::size_type indexDigit =
            s.find_first_not_of('0', exponentStartIndex);
            std::string exponent = "0";
            if (indexDigit !=
                std::string::npos) // There is an exponent different from 0
            {
                exponent = s.substr(indexDigit);
            }
            return normalized + exponent;
        }
        return s;
    }
    
    void printValueTree(Json::Value& value, const std::string& path="root", int depth=0) {
        string spaces = "";
        for(int i = 0; i < depth; i++) {
            spaces += "    ";
        }
        
        if (value.hasComment(Json::commentBefore)) {
            cout << spaces << value.getComment(Json::commentBefore).c_str() << endl;
        }
        
        cout << spaces << path.c_str() << ": ";
        
        switch (value.type()) {
            case Json::nullValue:
            {
                cout << "NULL" << endl;
                break;
            }
            case Json::intValue:
            {
                cout << "[int] " << Json::valueToString(value.asLargestInt()).c_str() << endl;
                break;
            }
            case Json::uintValue:
            {
                cout << "[uint] " << Json::valueToString(value.asLargestUInt()).c_str() << endl;
                break;
            }
            case Json::realValue:
            {
                cout << "[real] " << normalizeFloatingPointStr(value.asDouble()).c_str() << endl;
                break;
            }
            case Json::stringValue:
            {
                cout << "[string] " << value.asString().c_str() << endl;
                break;
            }
            case Json::booleanValue:
            {
                cout << "[boolean] " << (value.asBool() ? "true" : "false");
                break;
            }
            case Json::arrayValue:
            {
                cout << "[array] {" << endl;
                int size = value.size();
                for(int index = 0; index < size; ++index) {
                    static char buffer[16];
#if defined(_MSC_VER) && defined(__STDC_SECURE_LIB__)
                    sprintf_s(buffer, sizeof(buffer), "[%d]", index);
#else
                    snprintf(buffer, sizeof(buffer), "[%d]", index);
#endif
                    printValueTree(value[index], buffer, depth + 1);
                }
                cout << spaces << "}" << endl;
                break;
            }
            case Json::objectValue:
            {
                cout << "[object] {" << endl;
                Json::Value::Members members(value.getMemberNames());
                std::sort(members.begin(), members.end());
                for (Json::Value::Members::iterator it = members.begin();
                     it != members.end(); ++it) {
                    const std::string& name = *it;
                    printValueTree(value[name], name, depth + 1);
                }
                cout << spaces << "}" << endl;
                break;
            }
            default:
                cout << spaces << "I don't understand, it's too complicated." << endl;
        }
        
        if (value.hasComment(Json::commentAfter)) {
            cout << spaces << value.getComment(Json::commentAfter).c_str() << endl;
        }
    }
};