#include "src/prepared_statement.h"

namespace database {
    namespace mysql {

        prepared_statement::prepared_statement() {

        }

        prepared_statement::~prepared_statement() {

        }

        bool prepared_statement::prepare(connection& conn) {
            if (conn.getNativeHandler() != nullptr) {
                m_statement = mysql_stmt_init(conn.getNativeHandler());
            } else {
                m_statement = nullptr;
            }

            m_isBinded = false;

            if (m_statement != nullptr) {
                return init();
            }

            return false;
        }

        bool prepared_statement::execute() {
            return m_statement != nullptr && mysql_stmt_execute(m_statement) == 0;
        }

        bool prepared_statement::fetchRow() {
            int result = mysql_stmt_fetch(m_statement);
            mysql_stmt_free_result(m_statement);
            return m_statement != nullptr && result == 0;
        }

        long unsigned int prepared_statement::getParamResultLength(const size_t paramID) {
            if (paramID < m_resultInfo.size()) {
                return m_resultInfo[paramID].length;
            }

            return 0;
        }

        std::string prepared_statement::getErrorMsg() {
            if (m_statement != nullptr) {
                std::string result(mysql_stmt_error(m_statement));
                return result;
            }

            return "[FATAL] No statement found!";
        }

        bool prepared_statement::setQuery(const std::string query) {
            if (m_statement != nullptr && mysql_stmt_prepare(m_statement, query.c_str(), query.length()) == 0) {
                m_params.resize(mysql_stmt_param_count(m_statement));
                m_results.resize(mysql_stmt_field_count(m_statement));
                m_paramInfo.resize(m_params.size());
                m_resultInfo.resize(m_results.size());
                return true;
            }

            return false;
        }

        bool prepared_statement::bindParam() {
            return m_statement != nullptr && m_params.size() != 0 && mysql_stmt_bind_param(m_statement, m_params.data()) == 0;
        }

        bool prepared_statement::bindResult() {
            return m_statement != nullptr && m_results.size() != 0 && mysql_stmt_bind_result(m_statement, m_results.data()) == 0;
        }

    }
}



