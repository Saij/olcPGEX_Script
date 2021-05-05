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
	factor	: (PLUS|MINUS) factor | NUMBER | LPAREN expr RPAREN



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
#include <optional>
#include <ctype.h>
#include <iterator>
#pragma endregion

// O--------------------------------------------------------------------------O
// | olcPGEX_Script INTERFACE DECLARATION                                     |
// O--------------------------------------------------------------------------O
#pragma region pgex_script_declaration
namespace olc {
	/****************/
	/* Class Script */
	/****************/
	class ScriptEngine : olc::PGEX {
	public: 
		ScriptEngine() = default;

	public:
		bool LoadScript(std::string sScript);
	};

	namespace script {
		// Forward declarations for typedefs
		class Token;
		class Error;
		class ASTNode;

		using ASTNodeSharedPtr = std::shared_ptr<ASTNode>;
		using TokenValue = std::variant<std::monostate, int32_t>;
		using LexerReturn = std::variant<Token, Error>;
		using ParserReturn = std::variant<ASTNodeSharedPtr, Error>;

		/******************/
		/* Enum TokenType */
		/******************/
		enum class TokenType {
			TT_NONE,
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
			Token(TokenType type, TokenValue value);
			Token(TokenType type);
			Token();

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

		/*****************/
		/* Class ASTNode */
		/*****************/
		class ASTNode {
		protected:
			ASTNode() = default;

		public:
			virtual int Interpret() = 0;
		};

		/**********************/
		/* Class ASTBinOpNode */
		/**********************/
		class ASTBinOpNode : public ASTNode {
		public:
			ASTBinOpNode(ASTNodeSharedPtr leftNode, Token op, ASTNodeSharedPtr rightNode);

		public:
			int Interpret() override;

		private:
			ASTNodeSharedPtr m_leftNode;
			ASTNodeSharedPtr m_rightNode;
			Token m_op;
		};

		/************************/
		/* Class ASTUnaryOpNode */
		/************************/
		class ASTUnaryOpNode : public ASTNode {
		public:
			ASTUnaryOpNode(Token op, ASTNodeSharedPtr node);

		public:
			int Interpret() override;

		private:
			ASTNodeSharedPtr m_node;
			Token m_op;
		};

		/********************/
		/* Class ASTNumNode */
		/********************/
		class ASTNumNode : public ASTNode {
		public:
			ASTNumNode(Token num);

		public:
			int Interpret() override;

		private:
			Token m_num;
		};

		/***************/
		/* Class Error */
		/***************/
		class Error {
		public:
			Error(std::string sErrorName, std::string sErrorDescription);

		public:
			friend std::ostream& operator<<(std::ostream& os, const Error& error);

		protected:
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

		/******************************/
		/* Class UnexpectedTokenError */
		/******************************/
		class UnexpectedTokenError : public Error {
		public:
			UnexpectedTokenError(std::vector<TokenType> expected, TokenType got);
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

		/****************/
		/* Class Parser */
		/****************/
		class Parser {
		public:
			Parser(Lexer& lexer);
		public:
			ParserReturn Parse();

		private:
			ParserReturn Expr();
			ParserReturn Term();
			ParserReturn Factor();
			std::optional<Error> Eat(TokenType type);

		private:
			Lexer& m_lexer;
			Token m_currentToken;
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

	bool ScriptEngine::LoadScript(std::string sScript) {
		script::Lexer lexer(sScript);
		std::cout << "Loaded Script: " << sScript << std::endl;

		script::Parser parser(lexer);
		script::ParserReturn ret = parser.Parse();
		if (std::holds_alternative<script::Error>(ret)) {
			std::cout << "Error parsing script: " << std::get<script::Error>(ret) << std::endl;
			return false;
		}

		script::ASTNodeSharedPtr node = std::get<script::ASTNodeSharedPtr>(ret);
		int result = node->Interpret();

		std::cout << "Result: " << result << std::endl;
		
		return true;
	}

	namespace script {
		/**********************/
		/* Class ASTBinOpNode */
		/**********************/
		ASTBinOpNode::ASTBinOpNode(ASTNodeSharedPtr leftNode, Token op, ASTNodeSharedPtr rightNode)
			: m_leftNode(leftNode), m_op(op), m_rightNode(rightNode)
		{ }

		int ASTBinOpNode::Interpret()
		{
			switch (m_op.GetTokenType())
			{
			case TokenType::TT_PLUS:
				return m_leftNode->Interpret() + m_rightNode->Interpret();

			case TokenType::TT_MINUS:
				return m_leftNode->Interpret() - m_rightNode->Interpret();

			case TokenType::TT_MULTIPLY:
				return m_leftNode->Interpret() * m_rightNode->Interpret();

			case TokenType::TT_DIVIDE:
				return m_leftNode->Interpret() / m_rightNode->Interpret();

			default:
				return 0;
			}
		}

		/************************/
		/* Class ASTUnaryOpNode */
		/************************/
		ASTUnaryOpNode::ASTUnaryOpNode(Token op, ASTNodeSharedPtr node)
			: m_node(node), m_op(op)
		{ }

		int ASTUnaryOpNode::Interpret()
		{
			switch (m_op.GetTokenType())
			{
			case TokenType::TT_PLUS:
				return +m_node->Interpret();

			case TokenType::TT_MINUS:
				return -m_node->Interpret();

			default:
				return 0;
			}
		}

		/********************/
		/* Class ASTNumNode */
		/********************/
		ASTNumNode::ASTNumNode(Token numToken)
			: m_num(numToken)
		{ }

		int ASTNumNode::Interpret()
		{
			TokenValue num = m_num.GetValue();
			if (std::holds_alternative<int32_t>(num))
				return std::get<int32_t>(num);

			// @TOOD: Fire error
			return 0;
		}


		/***************/
		/* Class Token */
		/***************/
		Token::Token(TokenType type, TokenValue value) :
			m_type(type), m_value(value)
		{ }

		Token::Token(TokenType type) :
			m_type(type)
		{ }

		Token::Token() :
			Token(TokenType::TT_NONE)
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

			case TokenType::TT_NONE:
				os << "NONE";
				break;

			default:
				os << "UNKNOWN";
			}

			if (std::holds_alternative<int32_t>(token.m_value)) {
				int32_t nValue = std::get<int32_t>(token.m_value);
				os << ", " << nValue;
			}

			os << ")";
			return os;
		}

		/***************/
		/* Class Error */
		/***************/
		Error::Error(std::string sErrorName, std::string sErrorDescription) :
			m_sErrorName(sErrorName), m_sErrorDescription(sErrorDescription)
		{ }

		std::ostream& operator<< (std::ostream& os, const Error& error)
		{
			os << error.m_sErrorName << ": " << error.m_sErrorDescription;
			return os;
		}

		/**************************/
		/* Class IllegalCharError */
		/**************************/
		IllegalCharError::IllegalCharError(std::string sErrorDescription) :
			Error("IllegalCharError", sErrorDescription)
		{ }

		/******************************/
		/* Class UnexpectedTokenError */
		/******************************/
		UnexpectedTokenError::UnexpectedTokenError(std::vector<TokenType> vExpected, TokenType got) :
			Error("UnexpectedTokenError", "")
		{ 
			std::ostringstream ossDetail;

			ossDetail << "Expected token ";
			for (auto& expected : vExpected) {
				ossDetail << expected;

				if (&expected != &vExpected.back()) {
					ossDetail << ", ";
				}
			}

			ossDetail << " but got " << got << " instead";

			m_sErrorDescription = ossDetail.str();
		}

		/***************/
		/* Class Lexer */
		/***************/
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

			while (m_cCurrentChar != '\0' && token.GetTokenType() == TokenType::TT_NONE) {
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
					return IllegalCharError("'" + std::string(1, m_cCurrentChar) + "'");
				}

				Advance();
			}

			if (m_cCurrentChar == '\0' && token.GetTokenType() == TokenType::TT_NONE) {
				// End of script reached
				token.SetTokenType(TokenType::TT_EOF);
			}

			return token;
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
			return Token(TokenType::TT_NUMBER, nNumberValue);
		}

		/****************/
		/* Class Parser */
		/****************/
		Parser::Parser(Lexer& lexer)
			: m_lexer(lexer)
		{ }

		std::optional<Error> Parser::Eat(TokenType type)
		{
			if (m_currentToken.GetTokenType() == type) {
				LexerReturn ret = m_lexer.GetNextToken();
				if (std::holds_alternative<Error>(ret))
					return std::get<Error>(ret);

				m_currentToken = std::get<Token>(ret);
				return std::nullopt;
			}
			else {
				return UnexpectedTokenError({ type }, m_currentToken.GetTokenType());
			}
		}

		ParserReturn Parser::Parse()
		{
			// Current token is firstly set to TT_NONE
			std::optional<Error> error = Eat(TokenType::TT_NONE);
			if (error)
				return *error;

			ParserReturn ret = Expr();
			if (std::holds_alternative<Error>(ret))
				return std::get<Error>(ret);

			error = Eat(TokenType::TT_EOF);
			if (error)
				return *error;

			return ret;
		}

		ParserReturn Parser::Factor()
		{
			Token token = m_currentToken;
			TokenType curTokenType = token.GetTokenType();
			std::optional<Error> error;
			ParserReturn ret;

			switch (curTokenType)
			{
			case TokenType::TT_PLUS:
			case TokenType::TT_MINUS:
				error = Eat(curTokenType);
				if (error)
					return *error;

				ret = Factor();
				if (std::holds_alternative<Error>(ret))
					return std::get<Error>(ret);

				return std::make_shared<ASTUnaryOpNode>(token, std::get<ASTNodeSharedPtr>(ret));

			case TokenType::TT_NUMBER:
				error = Eat(curTokenType);
				if (error)
					return *error;

				return std::make_shared<ASTNumNode>(token);

			case TokenType::TT_LPAREN:
				error = Eat(TokenType::TT_LPAREN);
				if (error)
					return *error;

				ret = Expr();
				if (std::holds_alternative<Error>(ret))
					return std::get<Error>(ret);

				error = Eat(TokenType::TT_RPAREN);
				if (error)
					return *error;

				return std::get<ASTNodeSharedPtr>(ret);

			default:
				return UnexpectedTokenError({ TokenType::TT_LPAREN, TokenType::TT_NUMBER }, token.GetTokenType());
			}
		}

		ParserReturn Parser::Term()
		{
			std::optional<Error> error;

			ParserReturn ret = Factor();
			if (std::holds_alternative<Error>(ret))
				return std::get<Error>(ret);

			ASTNodeSharedPtr node = std::get<ASTNodeSharedPtr>(ret);

			while (m_currentToken.GetTokenType() == TokenType::TT_MULTIPLY || m_currentToken.GetTokenType() == TokenType::TT_DIVIDE) {
				Token op = m_currentToken;
				error = Eat(op.GetTokenType());

				if (error)
					return *error;

				ret = Factor();
				if (std::holds_alternative<Error>(ret))
					return std::get<Error>(ret);

				ASTNodeSharedPtr rightNode = std::get<ASTNodeSharedPtr>(ret);
				node = std::make_shared<ASTBinOpNode>(node, op, rightNode);
			}

			return node;
		}

		ParserReturn Parser::Expr()
		{
			std::optional<Error> error;

			ParserReturn ret = Term();
			if (std::holds_alternative<Error>(ret))
				return std::get<Error>(ret);

			ASTNodeSharedPtr node = std::get<ASTNodeSharedPtr>(ret);

			while (m_currentToken.GetTokenType() == TokenType::TT_PLUS || m_currentToken.GetTokenType() == TokenType::TT_MINUS) {
				Token op = m_currentToken;

				error = Eat(op.GetTokenType());
				if (error)
					return *error;

				ret = Term();
				if (std::holds_alternative<Error>(ret))
					return std::get<Error>(ret);

				ASTNodeSharedPtr rightNode = std::get<ASTNodeSharedPtr>(ret);
				node = std::make_shared<ASTBinOpNode>(node, op, rightNode);
			}

			return node;
		}
	}
}

#endif // OLC_PGEX_SCRIPT

#pragma endregion

#endif // OLC_PGEX_SCRIPT_H