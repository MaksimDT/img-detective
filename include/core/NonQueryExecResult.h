#pragma once

namespace ImgDetective {
namespace Core {
namespace Db {

    class NonQueryExecResult {
    public:
        NonQueryExecResult(unsigned long long lastInsertId)
            : lastInsertId(lastInsertId) {}
        NonQueryExecResult()
            : lastInsertId(0) {}

        unsigned long long GetLastInsertId() const {
            return lastInsertId;
        }
        void SetLastInsertId(unsigned long long lastInsertId) {
            this->lastInsertId = lastInsertId;
        }
    private:
        unsigned long long lastInsertId;
    };

}
}
}