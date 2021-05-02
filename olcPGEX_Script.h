#pragma region pgex_script_license_and_help
/*
	olcPGEX_Script.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                      Script v0.01                           |
	|                    Developed by Saij                        |
	+-------------------------------------------------------------+

	NOTE: UNDER ACTIVE DEVELOPMENT - THERE ARE BUGS/GLITCHES

	What is this?
	~~~~~~~~~~~~~

	This extensions implement a lexer, parser and interpreter for
	a custom designed scripting language to be used inside
	olcPixelGameEngine games



	Grammar Definition
	~~~~~~~~~~~~~~~~~~

	expr	: term ((PLUS|MINUS) term)*
	term	: factor ((MULTIPLY|DIVIDE) factor)*
	factor	: NUMBER | LPAREN expr RPAREN



	Defintion of numbers
	~~~~~~~~~~~~~~~~~~~~

	Currently there are only integers in the decimal system allowed.
	An integer can't start with the digit 0.
	Integers are stored in 32-bit signed variables



	License (OLC-3)
	~~~~~~~~~~~~~~~

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Author
	~~~~~~

	Christoph Friedrich, aka Saij
*/
#pragma endregion

#pragma region pgex_vesion_history
/*
	0.01: Initial release
*/
#pragma endregion

#pragma once
#ifndef OLC_PGEX_SCRIPT_H
#define OLC_PGEX_SCRIPT_H

// O--------------------------------------------------------------------------O
// | olcPGEX_Script STANDARD INCLUDES                                         |
// O--------------------------------------------------------------------------O
#pragma region pgex_script_includes
#include "olcPixelGameEngine.h"

#include <sstream>
#include <variant>
#include <ctype.h>
#pragma endregion

// O--------------------------------------------------------------------------O
// | olcPGEX_Script INTERFACE DECLARATION                                     |
// O--------------------------------------------------------------------------O
#pragma region pgex_script_declaration
namespace olc {
	/****************/
	/* Class Script */
	/****************/
	class Script : olc::PGEX {
	public: 
		Script() = default;

	public:
		bool LoadScript(std::string sScript);
	};

	namespace script {
		class Token;
		class Error;

		typedef std::variant<Token, Error> LexerReturn;

		/******************/
		/* Enum TokenType */
		/******************/
		enum class TokenType {
			TT_PLUS,
			TT_MINUS,
			TT_MULTIPLY,
			TT_DIVIDE,
			TT_LPAREN,
			TT_RPAREN,
			TT_EOF,
			TT_NUMBER
		};

		/***************/
		/* Class Token */
		/***************/
		typedef std::variant<std::monostate, std::string, int32_t> TokenValue;

		class Token {
		public:
			Token(TokenType type, TokenValue value);
			Token(TokenType type);
			Token();

		public:
			friend std::ostream& operator<<(std::ostream& os, const Token& token);

		public:
			TokenType GetTokenType();
			void SetTokenType(TokenType value);
			TokenValue GetValue();
			void SetValue(TokenValue value);

		private:
			TokenType m_type;
			TokenValue m_value;
		};

		/***************/
		/* Class Error */
		/***************/
		class Error {
		public:
			Error(std::string sErrorName, std::string sErrorDescription);

		public:
			friend std::ostream& operator<<(std::ostream& os, const Error& error);

		private:
			std::string m_sErrorName;
			std::string m_sErrorDescription;
		};

		/**************************/
		/* Class IllegalCharError */
		/**************************/
		class IllegalCharError : public Error {
		public:
			IllegalCharError(std::string sErrorDescription);
		};

		/***************/
		/* Class Lexer */
		/***************/
		class Lexer {
		public:
			Lexer(std::string sScript);

		public:
			LexerReturn GetNextToken();

		private:
			void Advance();
			LexerReturn GenerateNumberToken();

		private:
			char m_cCurrentChar;
			std::istringstream m_isScript;
		};
	}
}
#pragma endregion

// O--------------------------------------------------------------------------O
// | olcPGEX_Script IMPLEMENTATION                                            |
// O--------------------------------------------------------------------------O
#pragma region pgex_script_implementation

#ifdef OLC_PGEX_SCRIPT
#undef OLC_PGEX_SCRIPT

namespace olc {
	/****************/
	/* Class Script */
	/****************/
#pragma region pgex_script_impl_class

	bool Script::LoadScript(std::string sScript) {
		script::Lexer lexer(sScript);

		std::cout << "Loaded Script: " << sScript << std::endl;

		script::Token token;

		do {
			script::LexerReturn ret = lexer.GetNextToken();

			if (std::holds_alternative<script::Error>(ret)) {
				script::Error err = std::get<script::Error>(ret);
				std::cout << "Error: " << err << std::endl;
				break;
			}

			token = std::get<script::Token>(ret);

			std::cout << "Current Token: " << token << std::endl;
		} while (token.GetTokenType() != script::TokenType::TT_EOF);

		return true;
	}

#pragma endregion

	namespace script {
		/***************/
		/* Class Token */
		/***************/
#pragma region pgex_script_impl_token

		Token::Token(TokenType type, TokenValue value) :
			m_type(type), m_value(value)
		{ }

		Token::Token(TokenType type) :
			m_type(type)
		{ }

		Token::Token() :
			Token(TokenType::TT_EOF)
		{ }

		TokenType Token::GetTokenType()
		{
			return m_type;
		}

		void Token::SetTokenType(TokenType value)
		{
			m_type = value;
		}

		TokenValue Token::GetValue()
		{
			return m_value;
		}

		void Token::SetValue(TokenValue value)
		{
			m_value = value;
		}

		std::ostream& operator<< (std::ostream& os, const Token& token)
		{
			os << "Token(";
			switch (token.m_type) {
			case TokenType::TT_PLUS:
				os << "PLUS";
				break;

			case TokenType::TT_MINUS:
				os << "MINUS";
				break;

			case TokenType::TT_MULTIPLY:
				os << "MULTIPLY";
				break;

			case TokenType::TT_DIVIDE:
				os << "DIVIDE";
				break;

			case TokenType::TT_LPAREN:
				os << "LPAREN";
				break;

			case TokenType::TT_RPAREN:
				os << "RPAREN";
				break;

			case TokenType::TT_EOF:
				os << "EOF";
				break;

			case TokenType::TT_NUMBER:
				os << "NUMBER";
				break;

			default:
				os << "UNKNOWN";
			}

			if (std::holds_alternative<std::string>(token.m_value)) {
				std::string sValue = std::get<std::string>(token.m_value);
				os << ", \"" << sValue << "\"";
			}
			else if (std::holds_alternative<int32_t>(token.m_value)) {
				int32_t nValue = std::get<int32_t>(token.m_value);
				os << ", " << nValue;
			}

			os << ")";
			return os;
		}

#pragma endregion

		/***************/
		/* Class Error */
		/***************/
#pragma region pgex_script_impl_error

		Error::Error(std::string sErrorName, std::string sErrorDescription) :
			m_sErrorName(sErrorName), m_sErrorDescription(sErrorDescription)
		{ }

		std::ostream& operator<< (std::ostream& os, const Error& error)
		{
			os << error.m_sErrorName << ": " << error.m_sErrorDescription;
			return os;
		}

#pragma endregion

		/**************************/
		/* Class IllegalCharError */
		/**************************/
#pragma region pgex_script_impl_illegalcharerror

		IllegalCharError::IllegalCharError(std::string sErrorDescription) :
			Error("IllegalCharError", sErrorDescription)
		{ }

#pragma endregion

		/***************/
		/* Class Lexer */
		/***************/
#pragma region pgex_script_impl_lexer

		Lexer::Lexer(std::string sScript) :
			m_isScript(sScript)
		{ 
			Advance();
		}

		void Lexer::Advance()
		{
			m_cCurrentChar = m_isScript.get();

			if (m_isScript.eof()) {
				m_cCurrentChar = '\0';
			}
		}

		LexerReturn Lexer::GetNextToken()
		{
			static const std::string digits("123456789");
			Token token;

			while (m_cCurrentChar != '\0' && token.GetTokenType() == TokenType::TT_EOF) {
				if (m_cCurrentChar == '\t' || m_cCurrentChar == ' ') {
					// Ignore whitespaces
					Advance();
					continue;
				}
				else if (m_cCurrentChar == '+') {
					token.SetTokenType(TokenType::TT_PLUS);
				}
				else if (m_cCurrentChar == '-') {
					token.SetTokenType(TokenType::TT_MINUS);
				}
				else if (m_cCurrentChar == '*') {
					token.SetTokenType(TokenType::TT_MULTIPLY);
				}
				else if (m_cCurrentChar == '/') {
					token.SetTokenType(TokenType::TT_DIVIDE);
				}
				else if (m_cCurrentChar == '(') {
					token.SetTokenType(TokenType::TT_LPAREN);
				}
				else if (m_cCurrentChar == ')') {
					token.SetTokenType(TokenType::TT_RPAREN);
				}
				else if (isdigit(m_cCurrentChar)) {
					// Handle number tokens
					return GenerateNumberToken();
				}
				else {
					// Illegal character found!
					IllegalCharError error("'" + std::string(1, m_cCurrentChar) + "'");
					return LexerReturn(error);
				}

				Advance();
			}

			return LexerReturn(token);
		}

		LexerReturn Lexer::GenerateNumberToken()
		{
			if (m_cCurrentChar == '0') {
				IllegalCharError error("Leading zeros are not allowed for integer values");
				return LexerReturn(error);
			}

			std::string sNumber(1, m_cCurrentChar);
			Advance();

			while (m_cCurrentChar != '\0' && isdigit(m_cCurrentChar)) {
				sNumber += std::string(1, m_cCurrentChar);
				Advance();
			}

			int32_t nNumberValue = std::stoi(sNumber);
			Token numberToken(TokenType::TT_NUMBER, nNumberValue);

			return LexerReturn(numberToken);
		}

#pragma endregion
	}
}

#endif // OLC_PGEX_SCRIPT

#pragma endregion

#endif // OLC_PGEX_SCRIPT_H