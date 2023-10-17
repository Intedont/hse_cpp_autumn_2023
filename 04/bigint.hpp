class BigInt
{
private:
    int32_t* data = nullptr;
    size_t data_size;
    const std::int32_t base = 1000000000;
    const std::size_t base_power = 9;
    bool is_negative = false;
    static BigInt add(const BigInt& a, const BigInt& b);
    static BigInt subtract(const BigInt& a, const BigInt& b);
    static BigInt mult(const BigInt& a, const BigInt& b);
    static bool bigger_or_equal_abs(const BigInt& a, const BigInt& b);
    static bool bigger_abs(const BigInt& a, const BigInt& b);
    static void correct_length(BigInt& num);
public:
    BigInt(std::string num);
    BigInt(size_t);
    BigInt(const BigInt& source);
    BigInt(BigInt&& other);
    BigInt(int32_t source);
    BigInt();
    ~BigInt();
    int32_t* get_array() const;
    size_t get_size() const;
    std::string as_string() const;
    BigInt operator-() const;
    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other);
    friend BigInt operator + (const BigInt& a, const BigInt& b);
    friend BigInt operator + (const BigInt& a, const int32_t& b);
    friend BigInt operator - (const BigInt& a, const BigInt& b);
    friend BigInt operator - (const BigInt& a, const int32_t& b);
    friend BigInt operator * (const BigInt& a, const BigInt& b);
    friend BigInt operator * (const BigInt& a, const int32_t& b);
    friend std::ostream& operator << (std::ostream& out, const BigInt& num);
    friend bool operator == (const BigInt& a, const BigInt& b);
    friend bool operator != (const BigInt& a, const BigInt& b);
    friend bool operator > (const BigInt& a, const BigInt& b);
    friend bool operator >= (const BigInt& a, const BigInt& b);
    friend bool operator < (const BigInt& a, const BigInt& b);
    friend bool operator <= (const BigInt& a, const BigInt& b);
};