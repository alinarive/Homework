#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

enum class TokenType
{
        NULL_TOKEN = 0, // 0
    
        INT, STRING, BOOL,
        GOTO, LABEL,
        IF, ELSE,
        READ, WRITE,
        WHILE,
        AND, OR, NOT,
        START, FINAL, //15
    
        SEMICOLON, COLON, POINT, COMMA,
        ASSIGN,
        EQUAL, LESS, GREATER, NEQ, LEQ, GEQ,
        LEFT_BRACKET, RIGHT_BRACKET,
        BEGIN, END, //30
    
        PLUS, MINUS, MULTIPLY, DIVIDE,
    
        NUMBER,     //35
        IDENTIFIER, //36
        STRING_CONST, //37
        POLIZ_LABEL,
        POLIZ_ADDRESS,
        POLIZ_GO,
        POLIZ_FGO
};

class Identifier
{
private:
    std::string m_name;
    TokenType m_token_type;
    bool m_declare;
    bool m_assign;
    int m_value;
    static int s_id_count;
    int m_id_number;
    
public:
    Identifier():m_assign(false), m_declare(false)
    {
    }
    
    Identifier(const std::string& a_name):m_name(a_name), m_assign(false), m_declare(false)
    {
        m_id_number = s_id_count;
        ++s_id_count;
    }
    
    bool operator == (const std::string& a_string)
    {
        return m_name == a_string;
    }
    
    int get_id() const
    {
        return m_id_number;
    }
    
    std::string get_name() const
    {
        return m_name;
    }
    
    bool is_declared() const
    {
        return m_declare;
    }
    
    void put_declare()
    {
        m_declare = true;
    }
    
    TokenType get_type() const
    {
        return m_token_type;
    }
    
    void put_type(TokenType a_token_type)
    {
        m_token_type = a_token_type;
    }
       
    bool is_assigned() const
    {
        return m_assign;
    }
    
    void put_assign()
    {
        m_assign = true;
    }
    
    int get_value() const
    {
        return m_value;
    }
    
    void put_value(int a_value)
    {
        m_value = a_value;
    }
};

int Identifier::s_id_count{};

class Token
{
private:
    TokenType m_token_type;
    int m_value;
public:
    Token(TokenType a_token_type = TokenType::NULL_TOKEN, int a_value = 0)
    :m_token_type(a_token_type), m_value(a_value)
    {
    }

    TokenType get_type() const
    {
        return m_token_type;
    }

    int get_value() const
    {
        return m_value;
    }

    friend std::ostream& operator<<(std::ostream &a_output, const Token& a_token)
    {
        return a_output << '(' << (int)a_token.m_token_type << ',' << a_token.m_value << ");";
    }
};

std::vector <Identifier> IdentifierTable;

std::vector <std::string> StringTable;

int add_to_IT(const std::string &buffer)
{
    auto iterator_ID = std::find(IdentifierTable.begin(), IdentifierTable.end(), buffer);

    if(iterator_ID != IdentifierTable.end())
        return iterator_ID - IdentifierTable.begin();

    IdentifierTable.push_back(Identifier(buffer));
    return IdentifierTable.size() - 1;
}

class Scanner
{
private:
    std::ifstream m_file;
    char m_character;

    int table_lookup(const std::string& a_buffer, const std::string *a_table, int a_size)
    {
        for (int i = 0; i < a_size; ++i)
        {
            if (a_buffer == a_table[i])
                return i;
        }
        return 0;
    }

    void add_buffer(std::string &a_buffer)
    {
        a_buffer += m_character;
    }

    void get_character()
    {
        m_file.get(m_character);
    }

public:
    ~Scanner()
    {
        m_file.close();
    }

    static std::string service_word_table[], delimiter_table[];

    Scanner(const char *a_program)
    {
        m_file.open(a_program);
        if (!m_file.is_open())
        {
            throw std::runtime_error("[Scanner]: can't open file!");
        }
    }

    Token get_token();
};

std::string Scanner::service_word_table[] = {"not_indexed", "int", "string", "bool", "goto", "label", "if", "else",
                                           "read", "write", "while", "and", "or", "not", "program"};

std::string Scanner::delimiter_table[] = {"@", ";", ":", ".", ",", "=", "==", "<", ">", "!=", "<=",
                                         ">=", "(", ")", "{", "}", "+", "-", "*", "/"};

const int service_word_amount = 15, delimiter_amount = 20;

Token Scanner::get_token()
{
    enum class State
    {
        INITIALIZATION,
        IDENTIFIER,
        NUMBER,
        STRING,
        COMMENT,
        LESS_GREATER,
        NOT_EQUAL,
        DELIMITER
    };
    static int current_line{1};
    State current_state = State::INITIALIZATION;
    std::string symbol_buffer{};
    int number_buffer{}, in_table_number{};
    for(;;)
    {
        int is_comment = (m_character == '@');
        int is_assign = (m_character == '=');
        int is_string = (m_character == '"');
        get_character();
        switch (current_state)
        {
            case State::INITIALIZATION:
                symbol_buffer = std::string("");
                number_buffer = 0, in_table_number = 0;

                if (m_file.eof())
                {
                    return Token(TokenType::FINAL);
                }

                else if (std::isspace(m_character))
                {
                    current_line += (m_character == '\n');
                }
                else if (isalpha(m_character))
                {
                    symbol_buffer.push_back(m_character);
                    current_state = State::IDENTIFIER;
                }
                else if (isdigit(m_character))
                {
                    number_buffer = m_character - '0';
                    current_state = State::NUMBER;
                }
                else if (is_comment)
                {
                    current_state = State::COMMENT;
                }
                else if (is_string)
                {
                    current_state = State::STRING;
                }
                else if (m_character == ':' || m_character == '<' || m_character == '>')
                {
                    symbol_buffer.push_back(m_character);
                    current_state = State::LESS_GREATER;
                }
                else if (m_character == '!')
                {
                    symbol_buffer.push_back(m_character);
                    current_state = State::NOT_EQUAL;
                }
                else
                {
                    symbol_buffer.push_back(m_character);
                    if ((in_table_number = table_lookup(symbol_buffer, delimiter_table, delimiter_amount)))
                        return Token((TokenType) (in_table_number + (int) TokenType::FINAL), in_table_number);
                    else
                        throw m_character;
                }
                break;
            case State::IDENTIFIER:
                if (isalpha(m_character) || isdigit(m_character))
                {
                    symbol_buffer.push_back(m_character);
                }
                else
                {
                    m_file.unget();
                    if ((in_table_number = table_lookup(symbol_buffer, service_word_table, service_word_amount)))
                    {
                        return Token((TokenType) (in_table_number), in_table_number);
                    }
                    else
                    {
                        in_table_number = add_to_IT(symbol_buffer);
                        return Token(TokenType::IDENTIFIER, in_table_number);
                    }
                }
                break;
            case State::NUMBER:
                if (isdigit(m_character))
                {
                    number_buffer = number_buffer * 10 + (m_character - '0');
                }
                else
                {
                    m_file.unget();
                    return Token(TokenType::NUMBER, number_buffer);
                }
                break;
            case State::COMMENT:
                if (is_comment)
                {
                    current_state = State::INITIALIZATION;
                }
                else if (m_file.eof())
                {
                    throw std::runtime_error("[Scanner::getToken]: eof in the middle of comment!");
                }
                else
                {
                    current_line += (m_character == '\n');
                }
                break;
            case State::STRING:
                if (is_string)
                {
                    StringTable.push_back(symbol_buffer);
                    current_state = State::INITIALIZATION;
                    return Token(TokenType::STRING_CONST, StringTable.size() - 1);
                }
                else
                {
                    symbol_buffer.push_back(m_character);
                }
            case State::LESS_GREATER:
                if (is_assign)
                {
                    symbol_buffer.push_back(m_character);
                    in_table_number = table_lookup(symbol_buffer, delimiter_table, delimiter_amount);
                    return Token((TokenType) (in_table_number + (int) TokenType::FINAL), in_table_number);
                }
                else
                {
                    m_file.unget();
                    in_table_number = table_lookup(symbol_buffer, delimiter_table, delimiter_amount);
                    return Token((TokenType) (in_table_number + (int) TokenType::FINAL), in_table_number);
                }
                break;
            case State::NOT_EQUAL:
                if (is_assign)
                {
                    symbol_buffer.push_back(m_character);
                    in_table_number = table_lookup(symbol_buffer, delimiter_table, delimiter_amount);
                    return Token(TokenType::NEQ, in_table_number);
                }
                else
                {
                    throw std::runtime_error("[Scanner::getToken]: unexpected symbol '!'.");
                }
                break;
        }
    }
}

class Parser
{
private:
    Token m_token;
    TokenType m_token_type;
    int m_value;
    Scanner m_scanner;

    void get_token()
    {
        m_token = m_scanner.get_token();
        m_token_type = m_token.get_type();
        m_value = m_token.get_value();
    }

    void check_token(TokenType a_expected_type)
    {
        if(m_token_type != a_expected_type)
        {
            throw std::runtime_error("[Parser]: invalid type of token!");
        }
    }

    void get_token(TokenType a_expected_type)
    {
        get_token();
        check_token(a_expected_type);
    }

    void value()
    {
        if (m_token_type == TokenType::IDENTIFIER)
        {
            get_token();
        }
        else
        {
            constant();
            get_token();
        }
    }

    void multiplier_operand()
    {
        while (m_token_type == TokenType::NOT)
        {
            get_token();
        }

        while (m_token_type == TokenType::PLUS || m_token_type == TokenType::MINUS)
        {
            get_token();
        }

        if (m_token_type == TokenType::LEFT_BRACKET)
        {
            get_token();
            expression();

            check_token(TokenType::RIGHT_BRACKET);

            get_token();
        }
        else
        {
            value();
        }

    }

    void term_operand()
    {
        multiplier_operand();

        while (m_token_type == TokenType::MULTIPLY || m_token_type == TokenType::DIVIDE)
        {
            get_token();
            multiplier_operand();
        }
    }

    void compare_operand()
    {
        term_operand();

        while (m_token_type == TokenType::PLUS || m_token_type == TokenType::MINUS)
        {
            get_token();
            term_operand();
        }
    }

    void and_operand()
    {
        compare_operand();

        bool is_compare_operand = m_token_type >= TokenType::EQUAL &&
                                m_token_type <= TokenType::GEQ;

        if (is_compare_operand)
        {
            get_token();
            compare_operand();
        }
    }

    void or_operand()
    {
        and_operand();

        while (m_token_type == TokenType::AND)
        {
            get_token();
            and_operand();
        }
    }

    void assign_operand()
    {
        or_operand();

        while (m_token_type == TokenType::OR)
        {
            get_token();
            or_operand();
        }
    }

    void expression()
    {
        assign_operand();

        while (m_token_type == TokenType::ASSIGN)
        {
            get_token();
            assign_operand();
        }
    }

    void statement()
    {
        if (m_token_type == TokenType::READ)
        {
            get_token(TokenType::LEFT_BRACKET);
            {
                get_token(TokenType::IDENTIFIER);
            }
            get_token(TokenType::RIGHT_BRACKET);

            get_token(TokenType::SEMICOLON);
        }
        else if (m_token_type == TokenType::WRITE)
        {
            get_token(TokenType::LEFT_BRACKET);

            do
            {
                get_token();
                expression();
            }
            while (m_token_type == TokenType::COMMA);

            check_token(TokenType::RIGHT_BRACKET);

            get_token(TokenType::SEMICOLON);
        }
        else if (m_token_type == TokenType::WHILE)
        {
            get_token(TokenType::LEFT_BRACKET);
            {
                get_token();
                expression();
            }
            check_token(TokenType::RIGHT_BRACKET);
            {
                get_token();
                statement();
            }
        }
        else if (m_token_type == TokenType::IF)
        {
            get_token(TokenType::LEFT_BRACKET);
            {
                get_token();
                expression();
            }
            check_token(TokenType::RIGHT_BRACKET);
            {
                get_token();
                statement();
            }
            get_token(TokenType::ELSE);
            {
                get_token();
                statement();
            }
        }
        else if (m_token_type == TokenType::LABEL)
        {
            get_token(TokenType::COLON);
        }
        else if (m_token_type == TokenType::GOTO)
        {
            get_token(TokenType::LABEL);

            get_token(TokenType::SEMICOLON);
        }
        else if (m_token_type == TokenType::BEGIN)
        {
            get_token();
            statements();
        }
        else
        {
            expression();
            check_token(TokenType::SEMICOLON);
        }
    }

    void statements()
    {
        while (m_token_type != TokenType::END)
        {
            statement();
            get_token();
        }
    }

    void constant()
    {
        int is_add_subtraction = m_token_type == TokenType::MINUS
                            || m_token_type == TokenType::PLUS;

        int is_const = m_token_type == TokenType::NUMBER
                    && m_token_type == TokenType::STRING_CONST;

        if (is_add_subtraction)
        {
            get_token();

            if (m_token_type != TokenType::NUMBER)
            {
                throw std::runtime_error("[Parser]: expected number!");
            }
        }
        else if (!is_const)
        {
            throw std::runtime_error("[Parser]: expected constant!");
        }
    }

    void declarations()
    {
        auto is_declaration = m_token_type == TokenType::INT
                            || m_token_type == TokenType::STRING;

        while (is_declaration)
        {
            do
            {
                get_token(TokenType::IDENTIFIER);

                get_token();

                if (m_token_type == TokenType::ASSIGN)
                {
                    get_token();
                    constant();
                    get_token();
                }
            }
            while (m_token_type == TokenType::COMMA);

            check_token(TokenType::SEMICOLON);
            get_token();
        }
    }
public:
    Parser(const char *a_file)
            :m_scanner(a_file)
    {
    }

    void analyze()
    {
        get_token(TokenType::START);
        get_token(TokenType::BEGIN);
        {
            get_token();
            declarations();
            statements();
        }
        get_token(TokenType::END);
        get_token(TokenType::FINAL);
    }
};

int main()
{
    Token token{};
    try
    {
        Scanner scanner(R"(file.txt)");
        while (token.get_type() != TokenType::FINAL)
        {
            token = scanner.get_token();
            std::cout << token << '\n';
        }
        std::cout << "End of program.";
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}

