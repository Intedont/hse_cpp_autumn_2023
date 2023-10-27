#include <iostream>

enum class Error
{
    NoError,
    CorruptedArchive,
    UintOutOfRange
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out): out_(out) { 
        out_ << std::boolalpha;
    }

    template <class T>
    void save(T& object)
    {
        object.serialize(*this);
    }

    template <class... ArgsT>
    void operator()(ArgsT... args)
    {
        process(args...);
    }

    template <class First, class... Rest> 
    void process(const First& first, const Rest&... args){
        out_ << first << Separator;
        process(args...);
    }

private:
    std::ostream& out_;
    
    void process(){}
};


class Deserializer
{
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in): in_(in) { }

    template <class T>
    Error load(T& object)
    {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }

    Error process(bool& value)
    {
        std::string text;
        in_ >> text;

        if(text == "true"){
            value = true;
        } else if (text == "false"){
            value = false;
        } else {
            return Error::CorruptedArchive;
        }

        return Error::NoError;

    }

    Error process(uint64_t& value)
    {
        std::string text;
        in_ >> text;

        if(is_number(text)){
            char* end;
            value = std::strtoull(text.c_str(), &end, 10);
            // если число не влезло в uint64_t, то бросаем ошибку
            if (errno == ERANGE){
                errno = 0;
                return Error::UintOutOfRange;
            }
        } else {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

    template <class First, class... Rest> 
    Error process(First& first, Rest&... args){
        Error err = process(first);
        if(err != Error::NoError){
            return err;
        }
        err = process(args...);
        return err;
    }

private:
    std::istream& in_;

    void process(){}

    // Проверка, является ли считанная строка числом
    bool is_number(const std::string& s){
        if(s.size() != 0){
            for (char const &ch : s) {
                if (std::isdigit(ch) == 0) 
                    return false;
            }
            return true;
        } else {
            return false;
        }
    }
};