#include <iostream>
#include <vector>
#include <math.h>
#include <tuple> 
#include <functional> 

using namespace std;

int  fact(int n) {
    if(n==0 || n==1) return 1;
    for (int i = n - 1; i > 1; i--) {
        n *= i;
    }
    return n;
}

double p(int n, double p, int k) {
    return (fact(n) / (fact(n - k)*fact(k))) * pow(p, k) * pow(1 - p, n - k);
}

bool game(int rolls, int desired_face, int to_win) {
    int count = 0;
    for (int i = 0; i < rolls; i++) {
        int roll = rand() % 6 + 1;
        if (roll == desired_face) {
            count++;
        }
    }
    return count >= to_win;
}

tuple<int,int,double> basic_strategy(int k=5,int at_least=1,double initial_bet=5.0) {
    int win = 0;
    int lose = 0;
    double bet = initial_bet;
    double profit = 0.0;
    for(int i=0;i<=k;i++) {
        if(game(6, 3, at_least)) {
            win++;
            profit += bet;
            bet = initial_bet;
        } else {
            lose++;
            profit -= bet;
            bet *= 2;
        }
    }
    return make_tuple(win,lose,profit);
}
void sim(int n, int k, int at_least, function<tuple<int,int,double>(int,int,double)> strategy = basic_strategy) {
    int face_to_win = 3;
    cout << "Simulating the game of rolling a " << n << " die and face " << face_to_win << " in at least one of the die is needed to win: " << endl;
    vector<int> results(6,0);
    double initial_bet = 5.0;

    double total_profit = 0.0;
    for(int i=0;i<n;i++) {
        tuple<int,int,double> g = strategy(k,at_least,5.0);
        int win = get<0>(g);
        double profit = get<2>(g);
        total_profit += profit;
        results[win]++;
        // cout << "Total Profit after 5 games: " << profit << endl;
        // cout << "After 5 games: Wins: " << win << " Losses: " << lose << endl;
        // cout << "Win percentage: " << (double)win / (win + lose) * 100 << "%" << endl;
    }
    cout << "Average profit after 100 simulations of 5 games each (at least "<< at_least<<" needed to win): " << total_profit / (double) n << endl;

    cout << "Distribution of wins in 100 simulations of 5 games each (at least "<< at_least<<" needed to win):" << endl;
    for(int i=0;i<results.size();i++) {
        cout << i << " wins: " << results[i]/(double) n << endl;
    }
}

int main() {
    vector<double> pd;
    for (int i = 0; i <= 6; i++) {
        pd.push_back(p(6, 1.0 / 6.0, i));
        cout << "P(X=" << i << ") = " << pd[i] << endl;
    }

    double at_least_one = 1 - pd[0];

    cout << "Probability of at least one desired face: "<< at_least_one << endl;
   
    vector<double> pd_1;
    vector<double> pd_2;
    
    for(int i=0;i<=5;i++) {
        pd_1.push_back(p(5, at_least_one, i));
        cout << "P(X=" << i << ") = " << pd_1[i] << endl;
    }
    cout << "Expected wins (with at least 1 desired face) when rolling the die 5 to 100 times:" << endl;
    for(int i=5;i<=100;i+=5) {
        cout << i << " times played " << "expected win:" << i*at_least_one << endl;
    }

    double at_least_two = 1 - pd[0] - pd[1];
    cout << "Probability of at least two desired faces: " << at_least_two << endl;
    
    for(int i=0;i<=5;i++) {
        pd_2.push_back(p(5, at_least_two, i));
        cout << "P(X=" << i << ") = " << pd_2[i] << endl;
    }

    cout << "Expected wins (with at least 2 desired face) when rolling the die 5 to 100 times" << endl;
    for(int i=5;i<=100;i+=5) {
        cout << i << " times played " << "expected win:" << i*at_least_two << endl;
    }

    sim(100,5,1);
    
    sim(100,5,1, [](int k,int at_least,double initial_bet) -> tuple<int,int,double> {
        int win = 0;
        int lose = 0;
        double bet = initial_bet;
        double profit = 0.0;
        for(int i=0;i<=k;i++) {
            if(game(6, 3, at_least)) {
                win++;
                profit += bet;
                if(lose <= k/2+1) {
                    bet = initial_bet;
                }else{
                    bet *= 2; 
                }
            } else {
                lose++;
                profit -= bet;
                bet *= 2; 
            }
        }
        return make_tuple(win,lose,profit);
    });

    sim(100,5,1, [](int k,int at_least,double initial_bet) -> tuple<int,int,double> {
        int win = 0;
        int lose = 0;
        double bet = initial_bet;
        double profit = 0.0;
        for(int i=0;i<=k;i++) {
            if(game(6, 3, at_least)) {
                win++;
                profit += bet;
                if(lose <= k/2) {
                    bet = initial_bet;
                }else{
                    bet *= 2; 
                }
            } else {
                lose++;
                profit -= bet;
                bet *= 2; 
            }
        }
        return make_tuple(win,lose,profit);
    });

    return 0;
}