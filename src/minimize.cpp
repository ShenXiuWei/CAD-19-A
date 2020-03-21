
#include "datatype.h"
#include "satopt.h"
#include <assert.h>
#include<vector>
#include<utility>
#include<algorithm>
#include<iostream>
#define DEBUG 0
#define DUMP_NUM 1
#define DUMP_RATIO 0.1


using namespace::std;

void reorder(vec<Lit> &sol, vec<Lit> &high, vec<Lit> &low, vec<Lit> &result);
bool solve_a(Solver &solv, vec<Lit> &pat);
bool order_function(pair<int, int> i, pair<int, int> j)
{
    return i.second<j.second;

}
void input_reorder(vec<Lit> &pat, vector< pair<int,int> > &inputOrder, vec<Lit> &re_pat)
{
    for(int i = 0; i < inputOrder.size(); i++){
        if(sign(pat[inputOrder[i].first]) == 1){
            inputOrder[i].second = -inputOrder[i].second;
        }
    }
    sort(inputOrder.begin(),inputOrder.end(),order_function);
#if DEBUG
    cout<<"Pattern order : ";
    for(int i = 0;i < inputOrder.size(); i++)
        cout<< inputOrder[i].first<<" ";
    cout<<endl;
    for(int i = 0;i < inputOrder.size(); i++)
        cout<< inputOrder[i].second<<" ";
#endif
    for(int i = 0; i < inputOrder.size(); i++)
        re_pat.push(pat[inputOrder[i].first]);


}

void show_pattern(vec<Lit> & pat, int var_sign){
    if(var_sign == 0||var_sign == 2){// var
        for(int i =0 ;i< pat.size();i++)
            cout<<var(pat[i])<<" ";
        cout<<endl;
    }
    if(var_sign == 1 ||var_sign == 2){//sign
        for(int i =0 ;i< pat.size();i++)
            cout<<sign(pat[i])<<" ";
        cout<<endl;
    }
}

void minimize(Solver &soff, vec<Lit> &pat, vec<Lit> & result,vector<pair <int,int> > inputOrder, int method) {
    if(method == 1){
        pat.copyTo(result);
        for(int i = 0 ; i < pat.size(); i++){
            Lit n_var = result.last();
            result.pop();
            if(soff.solve(result)){// if off cnf is SAT means this assumption is needed
#if DEBUG 
                cout<<"needed : "<< var(pat[i])<<endl;
#endif
                result.push_front(n_var);
            }
        }
        return ;
    }
    else if(method == 4){
        vec<Lit> re_pat;
        input_reorder(pat, inputOrder, re_pat);
        re_pat.copyTo(result);
        for(int i = 0 ; i < re_pat.size(); i++){
            Lit n_var = result.last();
            result.pop();
            if(soff.solve(result)){// if off cnf is SAT means this assumption is needed
#if DEBUG 
                cout<<"needed : "<< var(pat[i])<<endl;
#endif
                result.push_front(n_var);
            }
        }
        //bubble sort for result before returm to satopt
        for(int i = 0; i < result.size() - 1;i++){
            for(int j = 0; j < result.size() -  i - 1; j++){
                if(var(result[j])>var(result[j+1])){
                    Lit temp = result[j];
                    result[j] = result[j+1];
                    result[j+1] = temp;
                }
            }
        }
        for(int i = 0; i < inputOrder.size(); i++){
            if(sign(pat[inputOrder[i].first]) == 1){
                inputOrder[i].second = -inputOrder[i].second;
            }
        }
        return ;
    }

    vec<Lit> current;
    vec<Lit> pat_high; 
    vec<Lit> pat_low;
    vec<Lit> sol_high;
    vec<Lit> sol_low;

    soff.copyTo(current);
#if DEBUG 
    cout<<"Start minimize"<<endl;  
    cout<<"Pattern : ";
    show_pattern(pat,0);  
    cout<<"Assumption right now : ";
    show_pattern(current,0);
#endif

    if (pat.size() == 1){
        if(soff.solve(current)){// solved without assumption, assumption needed
            //soff.assumptions.push(pat.last());
            pat.copyTo(result);
#if DEBUG 
            cout<<"Need assumption : "<<var(pat[0])<<endl;
#endif
            return;
        }else{
#if DEBUG 
            cout<<"No need assumption : "<<var(pat[0])<<endl;
#endif
            return;
        }

    }

    //Divide into high and low

    pat.copyTo(pat_high);

    for(int i = 0; i < pat.size()/2 ; i ++){
#if DEBUG 
        cout<<"Start dividing"<<endl;   
#endif
        pat_low.push_front(pat_high.last());
        pat_high.pop();
    }
#if DEBUG
    cout<<"HIGH : ";
    show_pattern(pat_high,1);
    cout<<"LOW  : ";
    show_pattern(pat_low,1);
#endif

    if(method == 2){
        if(!solve_a(soff, pat_low)){
            //return minimize(soff,pat_low);
#if DEBUG 
            cout<<"Search for assumptions in pat_low. pat_high are don't cares."<<endl;
#endif
            soff.copyFrom(current);
            minimize(soff,pat_low,result,inputOrder, method);
            return;
        }
        soff.copyFrom(current);
        if(!solve_a(soff, pat_high)){
            //return minimize(soff,pat_low);
#if DEBUG 
            cout<<"Search for assumptions in pat_high. pat_low are don't cares."<<endl;
#endif
            soff.copyFrom(current);
            minimize(soff,pat_high,result,inputOrder, method);
            return;
        }

        for(int i = 0; i< pat_low.size(); i++)
            current.push(pat_low[i]);
        soff.copyFrom(current);
        minimize(soff, pat_high, sol_high,inputOrder, method);
        for(int i = 0; i< pat_low.size(); i++)
            current.pop();
        for(int i = 0; i< sol_high.size(); i++)
            current.push(sol_high[i]);
        soff.copyFrom(current);
        minimize(soff,pat_low,sol_low,inputOrder, method);
    }
    else if(method == 3){
        if(!solve_a(soff, pat_low)){
#if DEBUG 
            cout<<"Search for assumptions in pat_low. pat_high are don't cares."<<endl;
#endif
            soff.copyFrom(current);
            minimize(soff,pat_low,result,inputOrder, method);
            return;
        }

        int mini = 10000;
        soff.copyFrom(current);
        for(int i = 0; i < pat_low.size(); i++){
            current.push(pat_low[i]);
            soff.copyFrom(current);
#if DEBUG 
            cout<<"Search for assumptions in pat_high. Increase"<<endl;
#endif
            minimize(soff, pat_high, result,inputOrder, method);
            if(result.size()<=mini){
                result.copyTo(sol_high);
                mini = result.size();
            }
        }
        for(int i = 0;i < pat_low.size(); i++){
            current.pop();
        }
        for(int i = pat_low.size() -1;i >= 0; i--){
            current.push(pat_low[i]);
        }
        for(int i = 0; i < pat_low.size(); i++){
            current.pop();
            soff.copyFrom(current);
#if DEBUG 
            cout<<"Search for assumptions in pat_high. Decrease"<<endl;
#endif
            minimize(soff, pat_high, result,inputOrder, method);    
            //minimize(soff,pat_high).copyTo(temp);
            if(result.size()<=mini){
                result.copyTo(sol_high);
                mini = result.size();
            }
        }
        //#endif

#if DEBUG 
        cout<<"sol_high : ";
        show_pattern(sol_high,1);
#endif
        mini = 10000;
        vec<Lit> sol_high_re;
        reorder(sol_high, pat_high, pat_low,sol_high_re); //questions
        /**** find sol_low after reordering ****/
        for(int i = 0; i < sol_high_re.size(); i++){
            current.push(sol_high_re[i]);
            soff.copyFrom(current);
#if DEBUG 
            cout<<"Search for assumptions in pat_low. Increase"<<endl;
#endif
            minimize(soff, pat_low, result,inputOrder, method);
            //minimize(soff,pat_low).copyTo(temp);
            if(result.size()<=mini){
                result.copyTo(sol_low);
                mini = result.size();
            }
        }
        for(int i = 0;i < sol_high_re.size(); i++){
            current.pop();
        }
        for(int i = sol_high_re.size() -1;i >= 0; i--){
            current.push(sol_high_re[i]);
        }
        for(int i = 0; i < sol_high_re.size(); i++){
            current.pop();
            soff.copyFrom(current);
#if DEBUG 
            cout<<"Search for assumptions in pat_low. Decrease"<<endl;
#endif
            minimize(soff, pat_low, result,inputOrder, method);
            //minimize(soff,pat_low).copyTo(temp);
            if(result.size()<=mini){
                result.copyTo(sol_low);
                mini = result.size();
            }
        }
    }
#if DEBUG 
    cout<<"sol_low : ";
    show_pattern(sol_low,1);
#endif


    sol_high.copyTo(result);
    for(int i = 0; i < sol_low.size(); i++){
        result.push(sol_low[i]);
    }
#if DEBUG 
    cout<<"result : ";
    show_pattern(result,1);
    cout<<"end mini"<<endl;
#endif
    return;
}

void reorder(vec<Lit> &sol, vec<Lit> &high, vec<Lit> &low, vec<Lit> &result)// remove part from A_low
{
    for(int i = 0; i < sol.size(); i++){
        int j = 0;
        for(j = 0; j < high.size(); j++){
            if(sol[i] == high[j]){
                result.push(sol[i]);
                break;
            }
        }
    }
    return;

}

bool solve_a(Solver &solv, vec<Lit> &pat)
{
    vec<Lit> assumptions;
    solv.copyTo(assumptions);
    for(int i = 0; i< pat.size(); i++)
        assumptions.push(pat[i]);
    return solv.solve(assumptions);
}

