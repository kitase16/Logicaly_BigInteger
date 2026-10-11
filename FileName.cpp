#include <iostream>
#include <cstdint>
#include <cfloat>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>


struct BYTE {
	union {
		struct {
			std::uint8_t _;
		};
		union {
			std::int8_t Signed;
			std::uint8_t UnSigned;
		};

	};
};

struct WORD {
	union {
		struct { BYTE A, B; };
		union {
			std::int16_t Signed;
			std::uint16_t UnSigned;
			BYTE Array[2]{};
		};
	};
};

struct BigInteger {
	std::vector<BYTE> Dight;
};

BigInteger MakeBigIntegerByBYTE(const BYTE& A){

	BigInteger R;

	R.Dight.push_back(A);

	return R;
}
	
bool IsZero(BigInteger A) {
	do {
		if (A.Dight.back().Signed == 0) { A.Dight.pop_back(); }
		else {
			return false;
		}
	} while (A.Dight.size() != 0);

	return true;
}

bool AandB_is_A_Big(const BigInteger& A,const BigInteger& B) {

	if (A.Dight.size() > B.Dight.size()) return true;


	for (auto rit=A.Dight.rbegin(), R = B.Dight.rbegin(); rit !=A.Dight.rend(); rit++)
	{
		if (rit->UnSigned < R->UnSigned) { return false; }


	}

	return true;


}


BigInteger Add(BigInteger A,BigInteger B) {
	BigInteger C;
		
	std::intmax_t L = std::min(A.Dight.size(), B.Dight.size());
	const BigInteger* D = (A.Dight.size() >= B.Dight.size()) ? &A : &B;

	A.Dight.push_back({ {0} });
	A.Dight.push_back({ {0} });
	A.Dight.push_back({ {0} });
	B.Dight.push_back({ {0} });
	B.Dight.push_back({ {0} });
	B.Dight.push_back({ {0} });
	for (std::intmax_t i = 0; i < L; i++) {
		const BYTE* BA = &A.Dight[i];
		const BYTE* BB = &B.Dight[i];
		WORD* WA = (WORD*)(BA);
		WORD* WB = (WORD*)(BB);
		WORD WC;
		WC.UnSigned = (WA->UnSigned) + (WB->UnSigned);
		C.Dight.push_back(WC.A);

	}

	A.Dight.pop_back();
	A.Dight.pop_back();
	A.Dight.pop_back();
	B.Dight.pop_back();
	B.Dight.pop_back();
	B.Dight.pop_back();

	for (std::size_t i = L; i < D->Dight.size(); i++) {
		const BYTE* BD = &D->Dight[i];
		const WORD* WD = (WORD*)(BD + 1);
		C.Dight.push_back(WD->B);//W=WD+0;

	}

	return C;


}
BigInteger Sub(BigInteger A,BigInteger B) {
	BigInteger C;


	
	std::intmax_t L = std::max(A.Dight.size(), B.Dight.size());
	const BigInteger* D = (A.Dight.size() >= B.Dight.size()) ? &A : &B;

	A.Dight.push_back({ {0} });
	A.Dight.push_back({ {0} });	
	A.Dight.push_back({ {0} });
	A.Dight.push_back({ {0} });
	A.Dight.push_back({ {0} });	
	A.Dight.push_back({ {0} });
	B.Dight.push_back({ {0} });
	B.Dight.push_back({ {0} });
	B.Dight.push_back({ {0} });
	B.Dight.push_back({ {0} });
	B.Dight.push_back({ {0} });
	B.Dight.push_back({ {0} });
	for (std::intmax_t i = 0; i < L; i++) {
		const BYTE* BA = &A.Dight[i];
		const BYTE* BB = &B.Dight[i];
		WORD* WA = (WORD*)(BA );
		WORD* WA2 = (WORD*)(BA + 1);
		WORD* WB = (WORD*)(BB );
		WORD WC;
		WC.UnSigned = (WA->UnSigned) - (WB->UnSigned);
		if (WA->Signed < 0) {
			WA->UnSigned += std::numeric_limits<decltype(WA->UnSigned)>::max();
			WA2->UnSigned--;
			i--;
			continue;
		}
		C.Dight.push_back(WC.A);

	}
	A.Dight.pop_back();
	A.Dight.pop_back();	
	A.Dight.pop_back();
	A.Dight.pop_back();
	A.Dight.pop_back();	
	A.Dight.pop_back();
	B.Dight.pop_back();
	B.Dight.pop_back();
	B.Dight.pop_back();
	B.Dight.pop_back();
	B.Dight.pop_back();
	B.Dight.pop_back();

	if (C.Dight.front().Signed < 0) { return MakeBigIntegerByBYTE({ {0} }); }

	return C;


}

BigInteger Mul(BigInteger A, BigInteger B) {
	BigInteger C = MakeBigIntegerByBYTE({ {0} });
	BigInteger D=B;

	do {
		C = Add(C, A);
		D=Sub(D, MakeBigIntegerByBYTE({ {1}, }));
	} while (!IsZero(D));

	return C;
}

BigInteger Div(BigInteger A, BigInteger B) {
	BigInteger C=A;
	BigInteger D = MakeBigIntegerByBYTE({ {0}, });

	do {
		C = Sub(C, B);
		D=Add(D, MakeBigIntegerByBYTE({ {1}, }));
	} while (AandB_is_A_Big(C,B));

	return D;
}

BigInteger Mod(BigInteger A, BigInteger B) {
	BigInteger C=A;
	BigInteger D;

	do {
		C = Sub(C, B);
		D=Add(D, MakeBigIntegerByBYTE({ {1}, }));
	} while (AandB_is_A_Big(C,B));

	return C;
}

std::string To_String(BigInteger A, BYTE Radix = { {10}, }) {
	std::string R;

	do {
		auto B = Mod(A, MakeBigIntegerByBYTE(Radix));
		A = Div(A, MakeBigIntegerByBYTE(Radix));

		R += std::to_string(B.Dight.front().UnSigned);

	} while (!IsZero(A));

	if (A.Dight.front().Signed < 0) { R += "-"; }


	std::reverse(R.begin(), R.end());

	return R;
}

BigInteger MakeBigIntegerFromString(const std::string& S, const BYTE Radix = { {10}, }) {
	BigInteger R = MakeBigIntegerByBYTE({ {0}, });
	std::string A = S;

	bool B = false;

	for (std::string::value_type& o : A) {

		R = Mul(R, MakeBigIntegerByBYTE(Radix));
		const char Ch[2]{ o,'\0' };
		if (o == '-') {
			B = true;
			continue;
		}
		if (o == ',') {
			continue;
		}
		BYTE X;
		X.UnSigned = std::atoi(Ch);
		R = Add(R, MakeBigIntegerByBYTE(X));
	}

	return R;
}


int main() {
	BigInteger A = MakeBigIntegerFromString("100");
	BigInteger B = MakeBigIntegerFromString("100");
	auto D = Add(A, B);
	auto C = To_String(D);


	std::cout << C << std::endl;

}