#include <iostream>
#include <vector>

// 중복되지 않는 요소를 두 개의 std::vector로 나누어 저장하는 함수
void findUnique(
    const std::vector<int>& singleVec,
    const std::vector<std::vector<int>>& combinedVec,
    std::vector<int>& uniqueInSingleVec,
    std::vector<int>& uniqueInCombinedVec
) {
    const int MAX_VAL = 10000;  // 예상 가능한 최대 값 (데이터 범위에 따라 조정 가능)
    bool isPresentInCombined[MAX_VAL] = {false};

    // combinedVec의 모든 요소를 기록
    for (int i = 0; i < combinedVec.size(); ++i) {
        for (int j = 0; j < combinedVec[i].size(); ++j) {
            isPresentInCombined[combinedVec[i][j]] = true;
        }
    }

    // singleVec에서 combinedVec에 없는 요소를 uniqueInSingleVec에 추가
    for (int i = 0; i < singleVec.size(); ++i) {
        if (!isPresentInCombined[singleVec[i]]) {
            uniqueInSingleVec.push_back(singleVec[i]);
        }
    }

    // combinedVec에서 singleVec에 없는 요소를 uniqueInCombinedVec에 추가
    for (int i = 0; i < combinedVec.size(); ++i) {
        for (int j = 0; j < combinedVec[i].size(); ++j) {
            int value = combinedVec[i][j];

            // singleVec에서 중복 확인
            bool foundInSingleVec = false;
            for (int k = 0; k < singleVec.size(); ++k) {
                if (value == singleVec[k]) {
                    foundInSingleVec = true;
                    break;
                }
            }

            // singleVec에 없는 경우만 추가
            if (!foundInSingleVec) {
                uniqueInCombinedVec.push_back(value);
            }
        }
    }
}

int main() {
    // 단일 벡터
    std::vector<int> singleVec = {5, 6, 7, 8};

    // 두 개의 벡터를 묶어서 combinedVec 생성
    std::vector<std::vector<int>> combinedVec = {
        {1, 2, 3},
        {4, 5, 6}
    };

    // 결과를 저장할 벡터
    std::vector<int> uniqueInSingleVec;   // singleVec에서 고유한 요소
    std::vector<int> uniqueInCombinedVec; // combinedVec에서 고유한 요소

    // 중복되지 않는 요소 찾기
    findUnique(singleVec, combinedVec, uniqueInSingleVec, uniqueInCombinedVec);

    // 결과 출력
    std::cout << "Unique in singleVec: ";
    for (int i = 0; i < uniqueInSingleVec.size(); ++i) {
        std::cout << uniqueInSingleVec[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Unique in combinedVec: ";
    for (int i = 0; i < uniqueInCombinedVec.size(); ++i) {
        std::cout << uniqueInCombinedVec[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
