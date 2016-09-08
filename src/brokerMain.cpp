#include <iostream>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;
#include <boost/atomic.hpp>
#include <boost/lockfree/queue.hpp>

typedef map<string,int> tradesOwner;//Map of trader and # of trades
typedef queue <tradesOwner> tradesQ; // Queue of tradesOwner

map<string, tradesQ > StockSellersLookup;
map<string, tradesQ > StockBuyersLookup;
bool isSellerinQueue = false;
bool isBuyerinQueue = false;
bool isStockEmpty = false;

bool matchingEngine(char buySell, int Trades, string StockName) {
    isSellerinQueue = false;
    isBuyerinQueue = false;

    if (buySell == 'B') {
        if (StockSellersLookup.find(StockName) != StockSellersLookup.end())
            isSellerinQueue = true;
    }
    if (StockBuyersLookup.find(StockName) != StockBuyersLookup.end())
        isBuyerinQueue = true;
    
    // Validation for single Queue either buyer or sellers. If both, then we failed
    if (isBuyerinQueue && isSellerinQueue) {
        cerr<<"Validation failed for Stock: "<<StockName<<endl;
        isStockEmpty = true;
        return false;
    }
    if (isBuyerinQueue || isSellerinQueue)
        return true;
    else
        return false;
    
}

void updateQ(map<string, tradesQ > * LookUp, int diff, string Stock, string Trader) {

        tradesQ buyerSellerQ;
        if (isStockEmpty == false) 
            buyerSellerQ = (*LookUp).find(Stock)->second;
        map <string, int> myMap;
        tradesOwner::iterator tempItr;
        if (buyerSellerQ.empty()) {
            // cout <<"Queue is empty"<<endl;
            myMap.insert(make_pair(Trader, diff));
            buyerSellerQ.push(myMap);
            (*LookUp).insert(make_pair(Stock, buyerSellerQ));
            tradesOwner mapTemp = buyerSellerQ.front();
                //mytradeOwners.push_back(mapTemp);
            tempItr = mapTemp.begin();
            // cout <<"Over here; "<<tempItr->first<< " has order of "<<tempItr->second<<" trades.\n";
        }
        else {
            tradesOwner mapTemp = buyerSellerQ.front();
            tempItr = mapTemp.begin();

            myMap.insert(make_pair(tempItr->first, diff));
            buyerSellerQ.pop(); // Update queue value
            (*LookUp).erase((*LookUp).find(Stock));
            buyerSellerQ.push(myMap);
            (*LookUp).insert(make_pair(Stock, buyerSellerQ));
        }
}

int processTradeHelper(map<string, tradesQ > * LookUp, string Stock, int trade) {
    int diff = 0;
    tradesQ buyerSellerQ;

    // cout <<"In Queue" <<endl;
    buyerSellerQ = (*LookUp).find(Stock)->second;
    tradesOwner::iterator tempItr;
    tradesOwner mapTemp = buyerSellerQ.front();
    tempItr = mapTemp.begin();
    buyerSellerQ.pop(); // Update queue value

    string traderName = tempItr->first;

    int tradeVal = tempItr->second;
    tempItr = mapTemp.end();
    (*LookUp).erase((*LookUp).find(Stock));
    // diff = tradeVal - trade;
    tradeVal -= trade;
    // cout <<"Diff Actual: "<<tradeVal<<endl;
    if (tradeVal <= 0) {
        cout <<"Success "<<traderName<<" "<<Stock<<endl;
        // cout <<"Checking Buyer : "<<matchingEngine('S', 1, "S1")<<endl;
        // cout <<"Checking Seller : "<<matchingEngine('S', 1, "S1")<<endl;
    } else {
        //Update this trader's queue
        updateQ(LookUp, diff, Stock, traderName);
    } 
    
    return tradeVal;
}

void processTrade(string Stock, int tradesRemain, char BuySellInd, string Trader){
    map<string, tradesQ > * tempLookUp = NULL;

    int origTrades = tradesRemain; 
    if (BuySellInd == 'B') { //Contact seller and make a sale
        if (isSellerinQueue == true) 
            tradesRemain = processTradeHelper(&StockSellersLookup, Stock, tradesRemain);

        //TODO: Update any remaining trades with Buyer, handle 0 trades also
        if (tradesRemain > 0)
            updateQ(&StockBuyersLookup, tradesRemain, Stock, Trader);
    } else {
        if (isBuyerinQueue == true) 
            tradesRemain = processTradeHelper(&StockBuyersLookup, Stock, tradesRemain);

        if (tradesRemain > 0)
            updateQ(&StockSellersLookup, tradesRemain, Stock, Trader);
    }
    if ((tradesRemain == 0) || (tradesRemain > origTrades))
    {
        cout <<"Success "<<Trader<<" "<<Stock<<endl;
    }
    // return tradesRemain;
}

void displayStock(string stName, char buySell) {
    map<string, tradesQ > * LookUp;
    tradesOwner::iterator tempItr;
    vector<tradesOwner> mytradeOwners;
    if (buySell == 'B') 
        LookUp = &StockBuyersLookup;
    else
        LookUp = &StockSellersLookup;

    tradesQ buyerSellerQ;
    buyerSellerQ = (*LookUp).find(stName)->second;


    if (!buyerSellerQ.empty()) {
        tradesOwner mapTemp = buyerSellerQ.front();
        mytradeOwners.push_back(mapTemp);
        tempItr = mapTemp.begin();
        cout <<tempItr->first<< " has order "<<buySell<< " of "<<tempItr->second \
             <<" trades of Stock"<<stName<<endl;;
        buyerSellerQ.pop();
    }
    for (tradesOwner myTO: mytradeOwners){
        buyerSellerQ.push(myTO);
    }
    mytradeOwners.clear();
}

int main() {
    int inputSize =  0;
    tradesQ BuyerQ; 
    tradesQ SellerQ;
    
    map <string, int> myMap;
    
    string traderName, stockName;
    int tradeCount;
    char buySellInd;

    cin >>inputSize>>ws;
    while(inputSize > 0) {
        cin>>traderName>>ws>>buySellInd>>ws>>tradeCount>>ws>>stockName;
        myMap.clear();

        // Check with Matching engine

        bool matchingEngineCheck = matchingEngine(buySellInd, tradeCount, stockName);
        // cout<<"Matching engine reports: "<< matchingEngineCheck <<endl;
        myMap.insert(make_pair(traderName, tradeCount));

        processTrade(stockName, tradeCount, buySellInd, traderName);
        #ifdef DEBUG
        displayStock(stockName, buySellInd);         
        #endif
        inputSize--;
    }


    return 0;
}

