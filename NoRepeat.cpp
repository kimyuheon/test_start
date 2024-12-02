#include <iostream>
#include <vector>

// 중복되지 않는 요소를 찾는 함수
std::vector<int> findUnique(const std::vector<std::vector<int>>& combinedVec, const std::vector<int>& singleVec) {
    std::vector<int> result;

    // combinedVec의 모든 요소를 확인
    for (int i = 0; i < combinedVec.size(); ++i) {
        for (int j = 0; j < combinedVec[i].size(); ++j) {
            int value = combinedVec[i][j];
            bool found = false;

            // singleVec에서 value가 있는지 확인
            for (int k = 0; k < singleVec.size(); ++k) {
                if (value == singleVec[k]) {
                    found = true;
                    break;
                }
            }

            // singleVec에 없는 값만 결과에 추가
            if (!found) {
                result.push_back(value);
            }
        }
    }

    // singleVec의 요소 중 combinedVec에 없는 요소 추가
    for (int i = 0; i < singleVec.size(); ++i) {
        bool found = false;

        // combinedVec에서 singleVec[i]가 있는지 확인
        for (int j = 0; j < combinedVec.size(); ++j) {
            for (int k = 0; k < combinedVec[j].size(); ++k) {
                if (singleVec[i] == combinedVec[j][k]) {
                    found = true;
                    break;
                }
            }
            if (found) break;
        }

        // combinedVec에 없는 값만 결과에 추가
        if (!found) {
            result.push_back(singleVec[i]);
        }
    }

    return result;
}

int main() {
    // 두 개의 벡터를 묶어서 combinedVec 생성
    std::vector<std::vector<int>> combinedVec = {
        {1, 2, 3},
        {4, 5, 6}
    };

    // 비교할 단일 벡터
    std::vector<int> singleVec = {5, 6, 7, 8};

    // 중복되지 않는 요소 찾기
    std::vector<int> uniqueElements = findUnique(combinedVec, singleVec);

    // 결과 출력
    std::cout << "Unique elements: ";
    for (int i = 0; i < uniqueElements.size(); ++i) {
        std::cout << uniqueElements[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
