#include <iostream>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;
//#include <boost/atomic.hpp>
//#include <boost/lockfree/queue.hpp>

typedef map<string,int> tradesOwner;//Map of trader and # of trades
typedef queue <tradesOwner> tradesQ; // Queue of tradesOwner


map<string, tradesQ > StockSellersLookup;
map<string, tradesQ > StockBuyersLookup;
bool isSellerinQueue = false;
bool isBuyerinQueue = false;

int getTradeDiff(string Stock) {
    return 1;// dummy for now
}
int matchingEngine(char buySell, int Trades, string StockName) {
    isSellerinQueue = false;
    isBuyerinQueue = false;

    if (buySell == 'B') {
        if (StockSellersLookup.find(StockName) != StockSellersLookup.end())
            isSellerinQueue = true;
    }
    if (StockBuyersLookup.find(StockName) != StockBuyersLookup.end())
        isBuyerinQueue = true;

    //return isSellerinQueue || isBuyerinQueue;  
    if (isSellerinQueue || isBuyerinQueue)
        //if ((isSellerinQueue == true) && (isBuyerinQueue == true)) 
            return getTradeDiff(StockName);
    else
        return 0; //TODO: Needs fixing
}

void processTradeHelper(bool *inQueue, map<string, tradesQ > * LookUp, string Stock, int trade) {
    tradesQ buyerSellerQ;
    if (*inQueue == true) {
        buyerSellerQ = (*LookUp).find(Stock)->second;
        tradesOwner::iterator tempItr;
        tradesOwner mapTemp = buyerSellerQ.front();
        tempItr = mapTemp.begin();
        buyerSellerQ.pop(); // Update queue value
        tempItr->second -= trade;
        if (tempItr->second <= 0) {
            cout <<"Success "<<tempItr->first<<" "<<Stock<<endl;
            tempItr->second = 0;
            (*LookUp).erase((*LookUp).find(Stock));
            // cout <<"Checking Buyer : "<<matchingEngine('S', 1, "S1")<<endl;
            // cout <<"Checking Seller : "<<matchingEngine('S', 1, "S1")<<endl;
        } else {
            // Update value in Queue and update LookUp
            map <string, int> myMap;
            myMap.insert(make_pair(tempItr->first, tempItr->second));
            buyerSellerQ.push(myMap);
        }
    }
}

void processTrade(string Stock, int trade){
    map<string, tradesQ > * tempLookUp = NULL;

    processTradeHelper(&isSellerinQueue, &StockSellersLookup, Stock, trade);
    processTradeHelper(&isBuyerinQueue, &StockBuyersLookup, Stock, trade);
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

        if (matchingEngineCheck == 0) { 
            map<string, tradesQ > * tempLookUp = NULL;
            if (buySellInd == 'B')
                tempLookUp = &StockBuyersLookup;
            else
                tempLookUp = &StockSellersLookup;
            
            if ((*tempLookUp).find(stockName) != (*tempLookUp).end()) {
                BuyerQ = (*tempLookUp).find(stockName)->second;
                BuyerQ.push(myMap);
            }
            else
            {
                BuyerQ.push(myMap);
                (*tempLookUp).insert(make_pair(stockName, BuyerQ));
            }

                
        } else {
            //Update seller and Buyer as applicable
            processTrade(stockName, tradeCount);
            //TODO: Add entry for remaining trades with Buyer/Seller
            //TODO: output success only when diff is 0.
            cout <<"Success "<<traderName<<" "<<stockName<<endl;
        }
        
        inputSize--;
    }


    return 0;
}

