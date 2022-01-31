#include <iostream>
#include <sstream>
#include <vector>
#include <optional>
#include <string>
#include <cstdlib>
#include <cmath>
#include <iomanip>

class Tokeniser
{
public:
    
    
    enum class Type
    {
        add,
        subtract,
        multiply,
        division,
        
        unknown,
    };
    
    
    
    struct TokenList
    {
        double lhs;
        double rhs;
        Type type;
    };

    std::optional <TokenList> tokenise (std::string input) const
    {
        auto type = findType (input);
        if (type == Type::unknown)
            return {};
            
        auto lhs = findLHS (input);
        if (! lhs.has_value ())
            return {};
            
        auto rhs = findRHS (input);
        if (! rhs.has_value ())
            return {};
    
        TokenList tokens;
        tokens.lhs = *lhs;
        tokens.rhs = *rhs;
        tokens.type = type;
        return tokens;
    }
    
private:

    bool find (std::string input, std::string character) const
    {
        return input.find (character) != std::string::npos;
    }
    
    double findConst(std::string input) const
    {
        //removes any spaces in string vers of consts
        input.erase(std::remove_if(input.begin(), input.end(), isspace));
        if (input == "pi")
            return 3.14159265;
        if (input == "root 2")
            return 1.41421;
        return 0.0;
    }
    
    
    
    std::optional <double> findAndExtractLHS (std::string input, std::string character) const
    {
        auto pos = input.find (character);
        auto anyConst = findConst(input.substr (0, pos));
        if (anyConst != 0.0)
            return anyConst;
        if (pos != std::string::npos)
            try{
                return std::stod (input.substr (0, pos));
            } catch(std::exception& e)
            {
                return{};
            }
            
        return {};
    }
    
    std::optional <double> findAndExtractRHS (std::string input, std::string character) const
    {
        auto pos = input.find (character);
        auto anyConst = findConst(input.substr (pos + 1));
        if (anyConst != 0.0)
            return anyConst;
        if (auto pos = input.find (character); pos != std::string::npos)
            try{
                return std::stod (input.substr (pos + 1));
            } catch(std::exception& e)
            {
                return{};
            }
        return {};
    }

    Type findType (std::string input) const
    {
        if (find (input, "+")) return Type::add;
        if (find (input, "-")) return Type::subtract;
        if (find (input, "*")) return Type::multiply;
        if (find (input, "/")) return Type::division;
        
        return Type::unknown;
    }
    
    std::optional <double> findLHS (std::string input) const
    {
        if (auto result = findAndExtractLHS (input, "+"))
            return result;
            
        if (auto result = findAndExtractLHS (input, "-"))
            return result;
            
        if (auto result = findAndExtractLHS (input, "*"))
            return result;
        
        if (auto result = findAndExtractLHS (input, "/"))
            return result;
            
        return {};
    }
    
    std::optional <double> findRHS (std::string input) const
    {
        if (auto result = findAndExtractRHS (input, "+"))
            return result;
            
        if (auto result = findAndExtractRHS (input, "-"))
            return result;
            
        if (auto result = findAndExtractRHS (input, "*"))
            return result;
        
        if (auto result = findAndExtractRHS (input, "/"))
            return result;
            
        return {};
    }
};

class Calculator
{
public:

    Calculator () = default;
    ~Calculator () = default;
    
    std::optional <double>  calculate (Tokeniser::TokenList tokens) const
    {
        switch (tokens.type)
        {
            case Tokeniser::Type::add:
                return tokens.lhs + tokens.rhs;
            case Tokeniser::Type::subtract:
                return tokens.lhs - tokens.rhs;
            case Tokeniser::Type::multiply:
                return tokens.lhs * tokens.rhs;
            case Tokeniser::Type::division:
                if (tokens.rhs == 0)
                    return {};
                return tokens.lhs / tokens.rhs;
            default:
                break;
        }
        
        return {};
    }
};

class InputProcessor
{
public:
    
    enum class InputRequest
    {
        quitProcessing,
        continueProcessing,
    };
    

    InputRequest requestInput () const
    {
        std::cout << "Please enter a calculation (Type Q to quit)" << std::endl;
        
        std::string input;
        std::getline (std::cin, input);
        
        if (input == "Q" || input == "q")
            return InputRequest::quitProcessing;
            
        processInput (input);
    
        return InputRequest::continueProcessing;
    }

private:

    void processInput (std::string input) const
    {
        if (auto tokens = Tokeniser ().tokenise (input))
            outputAnswer(Calculator ().calculate (*tokens));
        else
            std::cout << "There was an error in the input string, please try again..." << std::endl;
    }
    
    void outputAnswer(std::optional <double> possCalculatedAns) const
    {
        if(!possCalculatedAns.has_value ())
        {
            std::cout<< "No Answer.\n";
            return;
        }
        auto calculatedAns = possCalculatedAns.value();
        //check answer has decimal places
        auto ans = calculatedAns - int(calculatedAns);
        if(std::abs(ans) >= 0.00001)
        {
            //if so set decimal places to five
            std::cout << "Answer: " << std::setprecision(7) << calculatedAns << std::endl;
        } else
        {
            std::cout << "Answer: " << calculatedAns << std::endl;
        }
            
        
    }
};

class ResultChecker
{
public:

    static void check (double value, double expected, double range = 1e-3)
    {
        return assert (std::abs(value - expected) <= range);
    }

};

void test ()
{
    auto result = Tokeniser ().tokenise ("6*9");
    assert (result.has_value ());
    ResultChecker::check (result->lhs, 6);
    ResultChecker::check (result->rhs, 9);
    assert (result->type == Tokeniser::Type::multiply);
    
    result = Tokeniser ().tokenise ("6 * 9");
    assert (result.has_value ());
    ResultChecker::check (result->lhs, 6);
    ResultChecker::check (result->rhs, 9);
    assert (result->type == Tokeniser::Type::multiply);
    
    result = Tokeniser ().tokenise ("25 * 4");
    assert (result.has_value ());
    ResultChecker::check (result->lhs, 25);
    ResultChecker::check (result->rhs, 4);
    assert (result->type == Tokeniser::Type::multiply);
    
    result = Tokeniser ().tokenise("10/2");
    assert(result.has_value ());
    ResultChecker::check (result->lhs, 10);
    ResultChecker::check (result->rhs, 2);
    assert (result->type == Tokeniser::Type::division);
    
    result = Tokeniser ().tokenise("pi * 5");
    assert(result.has_value ());
    ResultChecker::check (result-> lhs, 3.14159265);
    ResultChecker::check (result-> rhs, 5);
    assert (result->type == Tokeniser::Type::multiply);
    
    

    ResultChecker::check (Calculator ().calculate ({ 10, 4, Tokeniser::Type::multiply }).value(), 40);
    ResultChecker::check (Calculator ().calculate ({ 25.3, 18.6, Tokeniser::Type::add }).value(), 43.9);
    ResultChecker::check (Calculator ().calculate ({ 3, 5.6, Tokeniser::Type::subtract }).value(), -2.6);
    //check pi*5
    ResultChecker::check (Calculator ().calculate ({ result->lhs, result->rhs, result->type}).value(), 15.70796);
    

}

void run ()
{
    InputProcessor processor;
    while (processor.requestInput () == InputProcessor::InputRequest::continueProcessing);
}

int main (int argc, const char * argv[])
{
    if (argc > 1 && std::string (argv[1]) == "--test")
        test ();
    else
        run ();
    
    return 0;
}
