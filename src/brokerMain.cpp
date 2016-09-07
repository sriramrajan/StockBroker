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

bool matchingEngine(char buySell, int Trades, string StockName) {
    isSellerinQueue = false;
    isBuyerinQueue = false;

    if (buySell == 'B') {
        if (StockSellersLookup.find(StockName) != StockSellersLookup.end())
            isSellerinQueue = true;
    }
    if (StockBuyersLookup.find(StockName) != StockBuyersLookup.end())
        isBuyerinQueue = true;

    return isBuyerinQueue || isSellerinQueue;
}

void updateQ(map<string, tradesQ > * LookUp, int diff, string Stock, string Trader) {

        tradesQ buyerSellerQ;

        buyerSellerQ = (*LookUp).find(Stock)->second;
        map <string, int> myMap;
        tradesOwner::iterator tempItr;
        if (buyerSellerQ.empty()) {
            myMap.insert(make_pair(Trader, diff));
            buyerSellerQ.push(myMap);
            (*LookUp).insert(make_pair(Stock, buyerSellerQ));
        }
        else {
            tradesOwner mapTemp = buyerSellerQ.front();
            tempItr = mapTemp.begin();
            tempItr->second = diff;
            myMap.insert(make_pair(tempItr->first, tempItr->second));
            buyerSellerQ.pop(); // Update queue value
            buyerSellerQ.push(myMap);
        }
}

int processTradeHelper(bool *inQueue, map<string, tradesQ > * LookUp, string Stock, int trade) {
    int diff = 0;
    tradesQ buyerSellerQ;
    if (*inQueue == true) {
        // cout <<"In Queue"<<endl;
        buyerSellerQ = (*LookUp).find(Stock)->second;
        tradesOwner::iterator tempItr;
        tradesOwner mapTemp = buyerSellerQ.front();
        tempItr = mapTemp.begin();
        buyerSellerQ.pop(); // Update queue value
        diff = abs(tempItr->second - trade);
        tempItr->second -= trade;
        if (tempItr->second <= 0) {
            cout <<"Success "<<tempItr->first<<" "<<Stock<<endl;
            (*LookUp).erase((*LookUp).find(Stock));
            // cout <<"Checking Buyer : "<<matchingEngine('S', 1, "S1")<<endl;
            // cout <<"Checking Seller : "<<matchingEngine('S', 1, "S1")<<endl;
        } else {
            // Update value in Queue and update LookUp
            map <string, int> myMap;
            myMap.insert(make_pair(tempItr->first, tempItr->second));
            buyerSellerQ.push(myMap);
            if (LookUp == &StockBuyersLookup)
                isBuyerinQueue = true;
        }
    }
    return diff;
}

int processTrade(string Stock, int trade, char BuySellInd){
    map<string, tradesQ > * tempLookUp = NULL;

    if (BuySellInd == 'B')
        return processTradeHelper(&isSellerinQueue, &StockSellersLookup, Stock, trade);
    else
        return processTradeHelper(&isBuyerinQueue, &StockBuyersLookup, Stock, trade);
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
            int Diff = processTrade(stockName, tradeCount, buySellInd);
            // cout <<"Diff1 :"<<Diff<<endl;
            if (Diff != 0) {
                if (buySellInd == 'B') {
                    updateQ(&StockSellersLookup, Diff, stockName, traderName);
                }
                else {
                    updateQ(&StockBuyersLookup, Diff, stockName, traderName);
                }
            }
            
            if (Diff == 0)
                cout <<"Success "<<traderName<<" "<<stockName<<endl;
        }
        
        inputSize--;
    }


    return 0;
}

