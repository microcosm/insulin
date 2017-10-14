#include "ofxJSON.h"

/*
 _id: [string] 5635ab0e1872c1a9b1bc752d
 date: [uint] 1445722752323
 dateString: [string] 2015-10-24T17:39:12.323-0400
 device: [string] xDrip-DexcomShare
 direction: [string] Flat
 filtered: [int] 152000
 noise: [int] 1
 rssi: [int] 100
 sgv: [int] 100
 type: [string] sgv
 unfiltered: [int] 148000
*/

class inJsonParser : public ofThread {
public:
    int bloodGlucoseValue = -1;
    bool newValueDetected = false;
    string className = "jsonParse";
protected:
    string lastId = "";
    string url = "https://andycgm.azurewebsites.net/api/v1/entries/sgv.json?find[dateString][$gte]=2015-08-28";
    ofxJSONElement response;
    Json::Value root, latestValidEntry;
    bool localNewValueDetected;

    void threadedFunction() {
        while(isThreadRunning()) {
            ofLogNotice(className) << "======> Thread awake. Total of " << ofGetElapsedTimeMillis() << "ms have elapsed.";
            if (response.open(url)) {
                ofLogNotice(className) << "JSON received. Validating and parsing...";
                root = (Json::Value) response;
                //printValueTree(root);
                extractLatestValues(root);
            } else {
                ofLogWarning(className) << "Couldn't open URL to retreive JSON.";
            }
            sleep(20000);
        }
    }

    void extractLatestValues(Json::Value& root) {
        if(extractLatestValidEntry(root)) {
            lock();
            bloodGlucoseValue = latestValidEntry["sgv"].asLargestInt();
            newValueDetected = localNewValueDetected = latestValidEntry["_id"].asString() != lastId;
            lastId = latestValidEntry["_id"].asString();
            unlock();
            ofLogNotice(className) << (localNewValueDetected ? "N" : "No n") << "ew reading detected.";
        }
    }

    bool extractLatestValidEntry(Json::Value& root) {
        if(validateArray(root)) {
            ofLogNotice(className) << "Root-level array is valid.";
        } else {
            return false;
        }
        
        for(int i = 0; i < root.size(); i++) {
            if(validateEntry(root[i], i)) {
                ofLogNotice(className) << "Entry " << i << " is valid. Assigning for use.";
                latestValidEntry = root[i];
                return true;
            }
        }
        return false;
    }

    bool validateArray(Json::Value& root) {
        if(root.type() != Json::arrayValue) {
            ofLogNotice(className) << "Root-level array not found. Aborting.";
            return false;
        } else if(root.size() < 1) {
            ofLogNotice(className) << "Found zero entries in root-level array. Aborting.";
            return false;
        }
        return true;
    }

    bool validateEntry(Json::Value& object, int i) {
        vector<string> members(object.getMemberNames());
        if(!contains(members, "type")) {
            ofLogNotice(className) << "Entry " << i << " doesn't contain required member 'type'.";
            return false;
        }
        if(!contains(members, "sgv")) {
            ofLogNotice(className) << "Entry " << i << " doesn't contain required member 'sgv'.";
            return false;
        }
        if(object["type"].asString() != "sgv") {
            ofLogNotice(className) << "Entry " << i << " member 'type' is '" << object["type"].asString() << "', not 'sgv'.";
            return false;
        }
        if(object["sgv"].asLargestInt() <= 20 || object["sgv"].asLargestInt() >= 999) {
            ofLogNotice(className) << "Entry " << i << " member 'sgv' is '" << object["sgv"].asString() << "', most likely a false reading.";
            return false;
        }
        return true;
    }

    bool contains(vector<string>& haystack, string needle) {
        return find(haystack.begin(), haystack.end(), needle) != haystack.end();
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
            ofLogNotice(className) << spaces << value.getComment(Json::commentBefore).c_str();
        }
        
        ofLogNotice(className) << spaces << path.c_str() << ": ";
        
        switch (value.type()) {
            case Json::nullValue:
            {
                ofLogNotice(className) << "NULL";
                break;
            }
            case Json::intValue:
            {
                ofLogNotice(className) << "[int] " << Json::valueToString(value.asLargestInt()).c_str();
                break;
            }
            case Json::uintValue:
            {
                ofLogNotice(className) << "[uint] " << Json::valueToString(value.asLargestUInt()).c_str();
                break;
            }
            case Json::realValue:
            {
                ofLogNotice(className) << "[real] " << normalizeFloatingPointStr(value.asDouble()).c_str();
                break;
            }
            case Json::stringValue:
            {
                ofLogNotice(className) << "[string] " << value.asString().c_str();
                break;
            }
            case Json::booleanValue:
            {
                ofLogNotice(className) << "[boolean] " << (value.asBool() ? "true" : "false");
                break;
            }
            case Json::arrayValue:
            {
                ofLogNotice(className) << "[array] {";
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
                ofLogNotice(className) << spaces << "}";
                break;
            }
            case Json::objectValue:
            {
                ofLogNotice(className) << "[object] {";
                Json::Value::Members members(value.getMemberNames());
                std::sort(members.begin(), members.end());
                for (Json::Value::Members::iterator it = members.begin();
                     it != members.end(); ++it) {
                    const std::string& name = *it;
                    printValueTree(value[name], name, depth + 1);
                }
                ofLogNotice(className) << spaces << "}";
                break;
            }
            default:
                ofLogNotice(className) << spaces << "I don't understand, it's too complicated.";
        }
        
        if (value.hasComment(Json::commentAfter)) {
            ofLogNotice(className) << spaces << value.getComment(Json::commentAfter).c_str();
        }
    }
};
