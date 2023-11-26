#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    Domain(const std::string&& domain) :domain_(std::move(domain)) {}

    bool operator ==(const Domain& other) const {
        return domain_ == other.domain_;
    }

    bool operator < (const Domain& other) const {
        return domain_ < other.domain_;
    }

    bool IsSubdomain(const Domain& other) const {
        if (domain_ == other.domain_) {
            return true;
        }

        if (other.domain_.size() <= domain_.size()) {
            return equal(other.domain_.begin(), other.domain_.end(), domain_.begin());
        }
        return false;
     }
private:
    string domain_;
};

class DomainChecker {
public:
    DomainChecker(std::vector<Domain>::const_iterator begin,
                  std::vector<Domain>::const_iterator end) : forbidden_domains_(begin, end) {
        sort(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
            return lhs < rhs;
            });
        auto indeterminate = unique(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
            return rhs.IsSubdomain(lhs);
            });
        forbidden_domains_.erase(indeterminate, forbidden_domains_.end());
    }

    bool IsForbidden(const Domain& domain) {
        if (forbidden_domains_.empty()) {
            return false;
        }

        auto it_domain = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain, [](const Domain& lhs, const Domain& rhs) {
            return lhs < rhs;
        });

        if (it_domain != forbidden_domains_.begin()) {
             return domain.IsSubdomain(forbidden_domains_[it_domain - forbidden_domains_.begin() - 1]);
        }
        else {
             return domain.IsSubdomain(forbidden_domains_[it_domain - forbidden_domains_.begin()]);
        }
        return false;
    }

private:
    std::vector<Domain> forbidden_domains_;
};

std::vector<Domain> ReadDomains(std::istream& in_streem, size_t number_domain) {
    std::vector<Domain> result;
    result.reserve(number_domain);

    for (size_t i = 0; i < number_domain; ++i) {
        string line;
        getline(in_streem, line);
        line.insert(0, 1, '.');
        reverse(line.begin(), line.end());
        result.emplace_back(move(line));
    }

    return result;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
