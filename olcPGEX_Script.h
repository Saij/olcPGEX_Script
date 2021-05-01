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
		class Token {
		public:
			Token(TokenType type, std::string sValue);
			Token(TokenType type);
			Token();

		public:
			friend std::ostream& operator<<(std::ostream& os, const Token& token);

		public:
			TokenType GetTokenType();
			void SetTokenType(TokenType value);
			std::string GetValue();
			void SetValue(std::string sValue);

		private:
			TokenType m_type;
			std::string m_sValue;
		};

		/***************/
		/* Class Error */
		/***************/
		class Error {
		public:
			Error(std::string sErrorName, std::string sErrorDescription);

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
				std::cout << "Error: " << std::endl;
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

		Token::Token(TokenType type, std::string sValue) :
			m_type(type), m_sValue(sValue)
		{ }

		Token::Token(TokenType type) :
			Token(type, "")
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

		std::string Token::GetValue()
		{
			return m_sValue;
		}

		void Token::SetValue(std::string sValue)
		{
			m_sValue = sValue;
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

			if (!token.m_sValue.empty()) {
				os << ", " << token.m_sValue;
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
			m_sErrorName(sErrorName), m_sErrorDescription(sErrorDesciption)
		{ }

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
			Token token;

			while (m_cCurrentChar != '\0' && token.GetTokenType() == TokenType::TT_EOF) {
				switch (m_cCurrentChar) {
				case '\t':
				case ' ':
					// Ignore whitespaces
					break;

				case '+':
					token.SetTokenType(TokenType::TT_PLUS);
					break;

				case '-':
					token.SetTokenType(TokenType::TT_MINUS);
					break;

				case '*':
					token.SetTokenType(TokenType::TT_MULTIPLY);
					break;

				case '/':
					token.SetTokenType(TokenType::TT_DIVIDE);
					break;

				case '(':
					token.SetTokenType(TokenType::TT_LPAREN);
					break;

				case ')':
					token.SetTokenType(TokenType::TT_RPAREN);
					break;

				default:
					std::string sVal(1, m_cCurrentChar);
					token.SetTokenType(TokenType::TT_NUMBER);
					token.SetValue(sVal);
					break;
				}

				Advance();
			}

			return LexerReturn(token);
		}

#pragma endregion
	}
}

#endif // OLC_PGEX_SCRIPT

#pragma endregion

#endif // OLC_PGEX_SCRIPT_H