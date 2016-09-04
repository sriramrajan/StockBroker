#include <iostream>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;
//#include <boost/atomic.hpp>
//#include <boost/lockfree/queue.hpp>

typedef map<string,int> tradesOwner;
typedef queue <tradesOwner> tradesQ;


map<string, tradesQ > StockSellersLookup;
map<string, tradesQ > StockBuyersLookup;


bool matchingEngine(char buySell, int Trades, string StockName) {
    
    if (buySell == 'B') {
        return (StockSellersLookup.find(StockName) != StockSellersLookup.end()); 
    }
    return (StockBuyersLookup.find(StockName) != StockBuyersLookup.end());

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
            /*
            if (buySellInd == 'B') {
            //cout <<StockBuyersLookup.find(stockName)->first<<endl;
            if (StockBuyersLookup.find(stockName) != StockBuyersLookup.end()) {
                BuyerQ = StockBuyersLookup.find(stockName)->second;
                BuyerQ.push(myMap);
            }
            else
            {
                BuyerQ.push(myMap);
                StockBuyersLookup.insert(make_pair(stockName, BuyerQ));
            }



            }
            else {
                //cout <<StockSellersLookup.find(stockName)->first<<endl;
            if (StockSellersLookup.find(stockName) != StockSellersLookup.end()){
                SellerQ = StockSellersLookup.find(stockName)->second;
                SellerQ.push(myMap);
            } else {
                SellerQ.push(myMap);
                StockSellersLookup.insert(make_pair(stockName, SellerQ));
                }

            }*/
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
            
        }
        inputSize--;
    }


    return 0;
}

