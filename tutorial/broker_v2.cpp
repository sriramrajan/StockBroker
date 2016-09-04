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

    return isSellerinQueue || isBuyerinQueue;  

}

void processTrade(string Stock, int trade){

    tradesQ sellerQ;
    if (isSellerinQueue == true) {
        sellerQ = StockSellersLookup.find(Stock)->second;
        tradesOwner::iterator tempItr;
        tradesOwner mapTemp = sellerQ.front();
        tempItr = mapTemp.begin();
        //sellerQ.pop();
        if (tempItr->second >= trade) {
            cout <<"Success "<<tempItr
->first<<" "<<Stock<<endl;
            
        }
    }
    
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
        cout<<"Matching engine reports: "<< matchingEngineCheck <<endl;
        myMap.insert(make_pair(traderName, tradeCount));

        if (matchingEngineCheck == false){ 
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
            //Place entry for balance traces if Seller is empty for stockName

            processTrade(stockName, tradeCount);

        }
        inputSize--;
    }


    return 0;
}

