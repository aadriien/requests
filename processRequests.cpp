#include <iostream>
#include <queue>
#include <map>

using namespace std;

// example request object coming from user
class Request {
    public:
        int requestVal;
        Request(int userRequest) {
            requestVal = userRequest;
        }
};


class ProcessRequests {
    public:
        const int MAX_IMMEDIATE = 3;
        const int MAX_WAITLIST = 1000;

        const int MAX_USER_REQ = 100;

        // immediate queue holds 3 new requests per sec, waitlist queue holds all others (capped)
        queue<Request> immediateReq;
        queue<Request> waitlistReq;       

        // mapping userID (key) to number of requests sent (value)
        map<int, int> userReqCount;

        bool filterRequest(Request currReq, int userID);
        void loadRequests();
        bool giveError();
};


/*
 * Purpose: confirm that there is capacity to process a new request
 * Notes: used in conjunction with loadRequests, called any time a user sends a request
*/
bool ProcessRequests::filterRequest(Request currReq, int userID) {
    // check curr user isn't maxing out requests
    if (userReqCount[userID] > MAX_USER_REQ) {
        return giveError();
    }

    // increment request count for curr user
    userReqCount[userID]++;

    // very few requests, can process immediately
    if (immediateReq.size() < MAX_IMMEDIATE) {
        immediateReq.push(currReq);
        return true;
    }
    // some preexisting requests, will process soon
    if (waitlistReq.size() < MAX_WAITLIST) {
        waitlistReq.push(currReq);
        return true;
    }
    // overloaded, user must wait a moment
    return giveError();
}


/*
 * Purpose: manage transmission of requests 
 * Notes: function itself must be called on set interval (of 3 loadRequests calls per second),
 *        exists independently of incoming user requests
*/
void ProcessRequests::loadRequests() {
    while (immediateReq.size() > 0) {
        // get next request in line
        Request sentRequest = immediateReq.front();
        immediateReq.pop();

        // get next request from waitlist & add to immediate
        immediateReq.push(waitlistReq.front());
        waitlistReq.pop();

        // note: front request can then be sent to another function as needed
        cout << "Success! Request " << sentRequest.requestVal << " has been passed." << endl;
    }
}


/*
 * Purpose: return error when there are too many requests
 * Notes: can be a response to individual user overload or multiple user overload
*/
bool ProcessRequests::giveError() {
    cerr << "The server is busy. Please try again in a moment." << endl;
    return false;
}


