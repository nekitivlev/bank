#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace bank {
struct transfer_error : std::runtime_error {
    explicit transfer_error(std::string const &a = "transfer_error")
        : std::runtime_error(a){};
};
struct not_enough_funds_error : transfer_error {
    explicit not_enough_funds_error()
        : transfer_error("not_enough_funds_error"){};
};
struct transaction;
struct user {
private:
    mutable std::mutex m_m;
    std::string username;
    int user_balance_xts = 0;
    std::vector<transaction> transtactions;

public:
    user() = default;
    explicit user(std::string const &name_);
    std::string name() const noexcept {
        return username;
    }
    int balance_xts() const {
        return user_balance_xts;
    }
    void snapshot_transactions(
        std::function<void(const std::vector<transaction> &transactions,
                           int balance)> const &input) const;
    void balance_change(bool mode, int amount_xts);
    void transfer(user &counterparty,
                  int amount_xts,
                  const std::string &comment);
};
struct transaction {
public:
    user *const
        counterparty;  // NOLINT(misc-non-private-member-variables-in-classes)
    const int
        balance_delta_xts;  // NOLINT(misc-non-private-member-variables-in-classes)
    const std::string
        comment;  // NOLINT(misc-non-private-member-variables-in-classes)
    transaction(user *const name, int balance, std::string coment)
        : counterparty(name),
          balance_delta_xts(balance),
          comment(std::move(coment)) {
    }
};

struct ledger {
private:
    std::map<std::string, user> users;
    mutable std::mutex m_m;

public:
    user &get_or_create_user(const std::string &name) {
        std::unique_lock l(m_m);
        users.emplace(name, name);
        return users[name];
    }
};

}  // namespace bank
