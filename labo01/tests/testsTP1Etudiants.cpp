/**
 * @file TestsEtudiants.cpp
 *
 * @brief Tests unitaires faits par l'étudiant pour la librairie d'algèbre linéaire.
 *
 * Nom: Frederic Labelle
 * Code permanent : LABF11109803
 * Email : frederic.labelle.3@ens.etsmtl.ca
 *
 */

#include "../src/Matrix.h"
#include "../src/Vector.h"
#include "../src/Math3D.h"
#include "../src/Operators.h"
#include "../src/DenseStorage.h"

#include <gtest/gtest.h>
#include <chrono>

using namespace gti320;
static void createSubMat(Matrix<double, Dynamic, Dynamic, RowStorage> mat, int top, int left, int n_rows, int n_cols);

// ===========================================================================================================
// ----- Tests supplémentaires évalués, à compléter! ---------------------------------------------------------
// ===========================================================================================================
#pragma region // Tests évalués

/**
 * DONE:
 * Écrivez trois tests supplémentaires pertinents, complets et bien commentés.
 * Seuls les trois tests de cette section seront évalués.
 *
 * Vous pouvez changer leurs noms, mais pas celui de la suite de tests.
 * Les tests évalués doivent être dans "TestsTP1_Evalue".
 *
 * Attention :
 * Le non-respect de ces consignes entraînera une note de zéro pour les tests.
 * 
 * Documentation :
 * https://google.github.io/googletest/primer.html
 */


/**
* Multiple tests on denseStorage, which is the base upon everything is built up on.
*/
TEST(TestsTP1_Evalue, TestDenseStorage)
{
    // Tests on static DenseStorage
    {
        // Size, setZero, read/write data
        DenseStorage<double, 5> ds;
        assert(ds.size() == 5);

        ds.setZero();
        double* data = ds.data();
        for (int i = 0; i < ds.size(); i++)
        {
            assert(*(data + i) == 0);
        }

        for (int i = 0; i < ds.size(); i++)
        {
            *(data + i) += i;
            assert(*(data + i) == i);
        }

        // copy constructor
        DenseStorage<double, 5> dsCopy(ds);
        assert(dsCopy.size() == ds.size());
        double* dataCopy = dsCopy.data();
        for (int i = 0; i < dsCopy.size(); i++)
        {
            assert(*(dataCopy + i) == *(data + i));
        }

        // copy operator
        DenseStorage<double, 5> dsCopyOperator;
        dsCopyOperator = ds;
        double* dataCopyOperator = dsCopyOperator.data();
        for (int i = 0; i < dsCopyOperator.size(); i++)
        {
            assert(*(dataCopyOperator + i) == *(data + i));
        }
    }

    // Tests on dynamic DenseStorage
    {
        // Size, read/write data
        DenseStorage<double, Dynamic> ds;
        assert(ds.data() == nullptr);
        assert(ds.size() == 0);

        DenseStorage<double, Dynamic> dsTaille(5);
        assert(dsTaille.size() == 5);

        double* dataTaille = dsTaille.data();
        for (int i = 0; i < dsTaille.size(); i++)
        {
            assert(*(dataTaille + i) == 0);
        }

        DenseStorage<double, Dynamic> dsCtrCopy(dsTaille);
        double* dataCtrCopy = dsCtrCopy.data();
        for (int i = 0; i < dsCtrCopy.size(); i++)
        {
            assert(*(dataCtrCopy + i) == *(dataTaille + i));
        }

        // Resize
        dsTaille.resize(2);
        assert(dsTaille.size() == 2);
        double* dataTailleResized = dsTaille.data();
        for (int i = 0; i < dsTaille.size(); i++)
        {
            assert(*(dataTailleResized + i) == 0);
        }

        // = operator
        DenseStorage<double, Dynamic> dsGros(5);
        DenseStorage<double, Dynamic> dsPetit(2);
        dsGros = dsPetit;
        assert(dsGros.size() == 2);

        DenseStorage<double, Dynamic> dsGros2(5);
        DenseStorage<double, Dynamic> dsPetit2(2);
        dsPetit2 = dsGros2;
        assert(dsPetit2.size() == 5);
    }
}

/**
* Tests on subMatrix construction and "()" access.
* They are lots of cases to cover since there are lots of parameter that can get you outOfBound
*/
TEST(TestsTP1_Evalue, TestSubMatrix)
{
    // Construct the Matrix from which we'll create subMatrix
    Matrix<double, Dynamic, Dynamic, RowStorage> RowM(2, 3);
    RowM.setZero();
    RowM(0, 0) = 1.0;
    RowM(0, 1) = 2.0;
    RowM(0, 2) = 3.0;
    RowM(1, 0) = 4.0;
    RowM(1, 1) = 5.0;
    RowM(1, 2) = 6.0;

    // Wrong construction
    EXPECT_DEATH(createSubMat(RowM, 2, 0, 1, 1), ""); // decalage i trop grand
    EXPECT_DEATH(createSubMat(RowM, 0, 3, 1, 1), ""); // decalage j trop grand
    EXPECT_DEATH(createSubMat(RowM, 1, 1, 2, 1), ""); // new rows too large
    EXPECT_DEATH(createSubMat(RowM, 0, 1, 1, 3), ""); // new cols too large


    // Wrong access
    /*
        [ 1 2 3
          4 5 6 ]
          => decalage 0, 1
        [ 2
          5 ]
    */
    const SubMatrix<double, Dynamic, Dynamic, RowStorage> sm(RowM, 0, 1, 2, 1);
    EXPECT_EQ(sm.rows(), 2);
    EXPECT_EQ(sm.cols(), 1);
    EXPECT_DOUBLE_EQ(sm(0, 0), 2.0);
    EXPECT_DOUBLE_EQ(sm(1, 0), 5.0);
    EXPECT_DEATH(sm(2, 0), ""); // https://google.github.io/googletest/advanced.html#death-tests 
    EXPECT_DEATH(sm(0, 1), ""); // not a coord of the submatrix
}

/**
* All cases of matrix multiplications (since given Tests were not testing all of them)
*/
TEST(TestsTP1_Evalue, TestMatrixMatrixMult)
{
    // Creating all types of matrix
    Matrix<double, Dynamic, Dynamic, RowStorage> A_row(3, 2);
    A_row(0, 0) = 1.0;
    A_row(0, 1) = 2.0;
    A_row(1, 0) = 3.0;
    A_row(1, 1) = 4.0;
    A_row(2, 0) = 5.0;
    A_row(2, 1) = 6.0;

    Matrix<double, Dynamic, Dynamic, ColumnStorage> B_col(2, 3);
    B_col(0, 0) = 1.0;
    B_col(0, 1) = 2.0;
    B_col(0, 2) = 3.0;
    B_col(1, 0) = 4.0;
    B_col(1, 1) = 5.0;
    B_col(1, 2) = 6.0;

    Matrix<double, Dynamic, Dynamic, ColumnStorage> A_col(3, 2);
    A_col(0, 0) = 1.0;
    A_col(0, 1) = 2.0;
    A_col(1, 0) = 3.0;
    A_col(1, 1) = 4.0;
    A_col(2, 0) = 5.0;
    A_col(2, 1) = 6.0;

    Matrix<double, Dynamic, Dynamic, RowStorage> B_row(2, 3);
    B_row(0, 0) = 1.0;
    B_row(0, 1) = 2.0;
    B_row(0, 2) = 3.0;
    B_row(1, 0) = 4.0;
    B_row(1, 1) = 5.0;
    B_row(1, 2) = 6.0;

    // row * col
    {
        auto C = A_row * B_col;   // 3x2 * 2*3 = 3x3

        EXPECT_DOUBLE_EQ(C(0, 0), 9.0);
        EXPECT_DOUBLE_EQ(C(0, 1), 12.0);
        EXPECT_DOUBLE_EQ(C(0, 2), 15.0);
        EXPECT_DOUBLE_EQ(C(1, 0), 19.0);
        EXPECT_DOUBLE_EQ(C(1, 1), 26.0);
        EXPECT_DOUBLE_EQ(C(1, 2), 33.0);
        EXPECT_DOUBLE_EQ(C(2, 0), 29.0);
        EXPECT_DOUBLE_EQ(C(2, 1), 40.0);
        EXPECT_DOUBLE_EQ(C(2, 2), 51.0);
    }

    // col * row
    {
        auto C = B_col * A_row; // 2x3 * 3*2 = 2x2

        EXPECT_DOUBLE_EQ(C(0, 0), 22.0);
        EXPECT_DOUBLE_EQ(C(0, 1), 28.0);
        EXPECT_DOUBLE_EQ(C(1, 0), 49.0);
        EXPECT_DOUBLE_EQ(C(1, 1), 64.0);
    }

    // generic row*row
    {
        auto C = A_row * B_row; // 3x2 * 2*3 = 3x3

        EXPECT_DOUBLE_EQ(C(0, 0), 9.0);
        EXPECT_DOUBLE_EQ(C(0, 1), 12.0);
        EXPECT_DOUBLE_EQ(C(0, 2), 15.0);
        EXPECT_DOUBLE_EQ(C(1, 0), 19.0);
        EXPECT_DOUBLE_EQ(C(1, 1), 26.0);
        EXPECT_DOUBLE_EQ(C(1, 2), 33.0);
        EXPECT_DOUBLE_EQ(C(2, 0), 29.0);
        EXPECT_DOUBLE_EQ(C(2, 1), 40.0);
        EXPECT_DOUBLE_EQ(C(2, 2), 51.0);
    }

    // generic col*col
    {
        auto C = A_col * B_col; // 3x2 * 2*3 = 3x3

        EXPECT_DOUBLE_EQ(C(0, 0), 9.0);
        EXPECT_DOUBLE_EQ(C(0, 1), 12.0);
        EXPECT_DOUBLE_EQ(C(0, 2), 15.0);
        EXPECT_DOUBLE_EQ(C(1, 0), 19.0);
        EXPECT_DOUBLE_EQ(C(1, 1), 26.0);
        EXPECT_DOUBLE_EQ(C(1, 2), 33.0);
        EXPECT_DOUBLE_EQ(C(2, 0), 29.0);
        EXPECT_DOUBLE_EQ(C(2, 1), 40.0);
        EXPECT_DOUBLE_EQ(C(2, 2), 51.0);
    }
}

#pragma endregion // Tests évalués

// ===========================================================================================================
// ----- Tests supplémentaires (Facultatif) -----------------------------------------------------------------
// ===========================================================================================================
#pragma region // Tests supplémentaires

/**
 * DONE: (Facultatif)
 * Vous pouvez utiliser cette section pour créer des tests supplémentaires.
 * Aucun de ces tests ne seront évalués.
 *
 * Vous pouvez changer leurs noms, mais pas celui de la suite de tests.
 * Les tests supplémentaires doivent être dans "TestLabo1_Etudiant".
 * 
 * Documentation :
 * https://google.github.io/googletest/primer.html
 */

using namespace gti320;

TEST(TestLabo1_Etudiant, Supplementaire1_matrixBase)
{
    MatrixBase<double, 2, 2> mb(3,3);
    assert(mb.size() == 4); // 2x2 and not 3x3, since the param ctr doesn't do anything with the values when col and row are static
    
    MatrixBase<double, Dynamic, 2> mbDynamicRow(3, 3);
    assert(mbDynamicRow.size() == 6); // 3 * 2(fixed)

    MatrixBase<double, 2, Dynamic> mbDynamicCol(1, 4);
    assert(mbDynamicCol.size() == 8); // 2(fixed) * 4

    MatrixBase<double, Dynamic, Dynamic> mbDynamic(3, 4);
    assert(mbDynamic.size() == 12); // 3*4
}


TEST(TestLabo1_Etudiant, Supplementaire1_vector_performance)
{
    using namespace std::chrono;

    Vector<double, Dynamic> myV;
    int size = 100000000;
    myV.resize(size);

    for (int i = 0; i < size; ++i)
    {
        myV(i) = i;
    }

    high_resolution_clock::time_point t = high_resolution_clock::now();

    myV.norm();
    myV.dot(myV);

    const duration<double> optimal_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);
    std::cout << "time: " << optimal_t.count() << '\n';

    // operator copy
    Vector<double> vCopy;
    vCopy = myV;
    assert(vCopy.size() == myV.size());
    assert(*(vCopy.data() + 2) == *(myV.data() + 2));
}

TEST(TestLabo1_Etudiant, Supplementaire1_ColMatrixCoordAccessNotSquareShaped)
{
    // Test - stockage par colonnes
    Matrix<double, Dynamic, Dynamic, ColumnStorage> ColM(21, 36);
    ColM.setZero();
    ColM(0, 0) = 1.0;
    ColM(13, 28) = 99.0;
    ColM(10, 33) = 5.0;
    EXPECT_EQ(ColM(0, 0), 1.0);
    EXPECT_EQ(ColM(10, 33), 5.0);
    EXPECT_EQ(ColM(13, 28), 99.0);
}

TEST(TestLabo1_Etudiant, Supplementaire1_MatrixTranspose)
{
    Matrix<double, Dynamic, Dynamic, RowStorage> RowM(2, 3);
    RowM.setZero();
    RowM(0, 0) = 1.0;
    RowM(0, 1) = 2.0;
    RowM(0, 2) = 3.0;
    RowM(1, 0) = 4.0;
    RowM(1, 1) = 5.0;
    RowM(1, 2) = 6.0;

    Matrix<double, Dynamic, Dynamic, ColumnStorage> colTransposedFromRow = RowM.transpose();

    // Row -> Col
    EXPECT_EQ(colTransposedFromRow.rows(), 3);
    EXPECT_EQ(colTransposedFromRow.cols(), 2);
    EXPECT_DOUBLE_EQ(colTransposedFromRow(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(colTransposedFromRow(0, 1), 4.0);
    EXPECT_DOUBLE_EQ(colTransposedFromRow(1, 0), 2.0);
    EXPECT_DOUBLE_EQ(colTransposedFromRow(1, 1), 5.0);
    EXPECT_DOUBLE_EQ(colTransposedFromRow(2, 0), 3.0);
    EXPECT_DOUBLE_EQ(colTransposedFromRow(2, 1), 6.0);


    // Col -> Row
    auto transposedColMatrixBackToRowMatrix = colTransposedFromRow.transpose<double, 2, 3, RowStorage>();

    EXPECT_EQ(transposedColMatrixBackToRowMatrix.rows(), 2);
    EXPECT_EQ(transposedColMatrixBackToRowMatrix.cols(), 3);
    EXPECT_DOUBLE_EQ(transposedColMatrixBackToRowMatrix(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(transposedColMatrixBackToRowMatrix(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(transposedColMatrixBackToRowMatrix(0, 2), 3.0);
    EXPECT_DOUBLE_EQ(transposedColMatrixBackToRowMatrix(1, 0), 4.0);
    EXPECT_DOUBLE_EQ(transposedColMatrixBackToRowMatrix(1, 1), 5.0);
    EXPECT_DOUBLE_EQ(transposedColMatrixBackToRowMatrix(1, 2), 6.0);

    // Col -> Col
    Matrix<double, 2, 3, ColumnStorage> colTransposedFromCol = colTransposedFromRow.transpose<double, 2, 3, ColumnStorage>();
    EXPECT_EQ(colTransposedFromCol.rows(), 2);
    EXPECT_EQ(colTransposedFromCol.cols(), 3);
    EXPECT_DOUBLE_EQ(colTransposedFromCol(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(colTransposedFromCol(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(colTransposedFromCol(0, 2), 3.0);
    EXPECT_DOUBLE_EQ(colTransposedFromCol(1, 0), 4.0);
    EXPECT_DOUBLE_EQ(colTransposedFromCol(1, 1), 5.0);
    EXPECT_DOUBLE_EQ(colTransposedFromCol(1, 2), 6.0);
}

TEST(TestLabo1_Etudiant, Supplementaire1_IdentityWithRowMatrix)
{
    // Test : matrice identité avec RowStorage
    Matrix<double, Dynamic, Dynamic, RowStorage> A(6, 6);
    A.setIdentity();
    EXPECT_DOUBLE_EQ(A(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(A(1, 1), 1.0);
    EXPECT_DOUBLE_EQ(A(2, 2), 1.0);
    EXPECT_DOUBLE_EQ(A(3, 3), 1.0);
    EXPECT_DOUBLE_EQ(A(4, 4), 1.0);
    EXPECT_DOUBLE_EQ(A(5, 5), 1.0);
    EXPECT_DOUBLE_EQ(A(0, 1), 0.0);
    EXPECT_DOUBLE_EQ(A(1, 0), 0.0);
}


static void createSubMat(Matrix<double, Dynamic, Dynamic, RowStorage> mat, int top, int left, int n_rows, int n_cols)
{
    const SubMatrix<double, Dynamic, Dynamic, RowStorage> sm(mat, top, left, n_rows, n_cols);
}



TEST(TestLabo1_Etudiant, Supplementaire1_SubMatrixEqOp)
{
    Matrix<double, Dynamic, Dynamic, RowStorage> RowM(2, 3);
    RowM.setZero();
    RowM(0, 0) = 1.0;
    RowM(0, 1) = 2.0;
    RowM(0, 2) = 3.0;
    RowM(1, 0) = 4.0;
    RowM(1, 1) = 5.0;
    RowM(1, 2) = 6.0;

    // operator= with rowMatrix
    Matrix<double, Dynamic, Dynamic, RowStorage> B(2, 1);
    B = RowM.block(0, 1, 2, 1);
    EXPECT_EQ(B.rows(), 2);
    EXPECT_EQ(B.cols(), 1);
    EXPECT_DOUBLE_EQ(B(0, 0), 2.0);
    EXPECT_DOUBLE_EQ(B(1, 0), 5.0);

    // operator= with colMatrix (with a resize)
    Matrix<double, Dynamic, Dynamic, ColumnStorage> C(2, 1);
    C= RowM.block(0, 1, 1, 1);
    EXPECT_EQ(C.rows(), 1);
    EXPECT_EQ(C.cols(), 1);
    EXPECT_DOUBLE_EQ(C(0, 0), 2.0);

    // Operator= subMatrix = Matrix  (not same size)
    SubMatrix<double, Dynamic, Dynamic, RowStorage> sub(RowM, 1, 1, 1, 2); // sub doesn't have the same size as RowM -> assertion fail
    EXPECT_DEATH(sub = RowM, "");

    // Operator= subMatrix = Matrix  (same size)
    Matrix<double, Dynamic, Dynamic, RowStorage> matrixBidon(2, 3);
    SubMatrix<double, Dynamic, Dynamic, RowStorage> sub2(matrixBidon, 0,0,2,3);
    sub2 = RowM;

    EXPECT_DOUBLE_EQ(sub2(0, 0) , 1.0);
    EXPECT_DOUBLE_EQ(sub2(0, 1) , 2.0);
    EXPECT_DOUBLE_EQ(sub2(0, 2) , 3.0);
    EXPECT_DOUBLE_EQ(sub2(1, 0) , 4.0);
    EXPECT_DOUBLE_EQ(sub2(1, 1) , 5.0);
    EXPECT_DOUBLE_EQ(sub2(1, 2) , 6.0);
}


TEST(TestLabo1_Etudiant, Supplementaire1_Sums)
{
    // Vect + Vect
    Vector<> v1(3);
    v1(0) = 1.0;
    v1(1) = 2.0;
    v1(2) = 3.0;
    Vector<> v2(3);
    v2(0) = 4.0;
    v2(1) = 5.0;
    v2(2) = 6.0;
    Vector<> v1_plus_v2 = v1 + v2;
    EXPECT_DOUBLE_EQ(v1_plus_v2(0), 5.0);
    EXPECT_DOUBLE_EQ(v1_plus_v2(1), 7.0);
    EXPECT_DOUBLE_EQ(v1_plus_v2(2), 9.0);

    // Vect - Vect
    Vector<> v1_minus_v2 = v1 - v2;
    EXPECT_DOUBLE_EQ(v1_minus_v2(0), -3.0);
    EXPECT_DOUBLE_EQ(v1_minus_v2(1), -3.0);
    EXPECT_DOUBLE_EQ(v1_minus_v2(2), -3.0);

    //// Mat + Mat (generic)
    Matrix<double, Dynamic, Dynamic, RowStorage> A(3, 2);
    A(0, 0) = 2.0;
    A(0, 1) = 1.0;
    A(1, 0) = -2.5;
    A(1, 1) = 6.5;
    A(2, 0) = 1.5;
    A(2, 1) = 0.5;
    Matrix<double, Dynamic, Dynamic, ColumnStorage> B(3, 2);
    B(0, 0) = 2.0;
    B(0, 1) = 1.0;
    B(1, 0) = -2.5;
    B(1, 1) = 6.5;
    B(2, 0) = 1.5;
    B(2, 1) = 0.5;

    const auto A_plus_B = A + B;
    EXPECT_DOUBLE_EQ(A_plus_B(0, 0), 4);
    EXPECT_DOUBLE_EQ(A_plus_B(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(A_plus_B(1, 0), -5.0);
    EXPECT_DOUBLE_EQ(A_plus_B(1, 1), 13.0);
    EXPECT_DOUBLE_EQ(A_plus_B(2, 0), 3.0);
    EXPECT_DOUBLE_EQ(A_plus_B(2, 1), 1.0);

    // MatRow + MatRow
    const auto A_plus_A = A + A;
    EXPECT_DOUBLE_EQ(A_plus_A(0, 0), 4);
    EXPECT_DOUBLE_EQ(A_plus_A(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(A_plus_A(1, 0), -5.0);
    EXPECT_DOUBLE_EQ(A_plus_A(1, 1), 13.0);

    // MatCol + MatCol
    Matrix<double, Dynamic, Dynamic, ColumnStorage> ACol(2, 2);
    ACol(0, 0) = 2.0;
    ACol(0, 1) = 1.0;
    ACol(1, 0) = -2.5;
    ACol(1, 1) = 6.5;

    const auto A_plus_ACol = ACol + ACol;
    EXPECT_DOUBLE_EQ(A_plus_ACol(0, 0), 4);
    EXPECT_DOUBLE_EQ(A_plus_ACol(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(A_plus_ACol(1, 0), -5.0);
    EXPECT_DOUBLE_EQ(A_plus_ACol(1, 1), 13.0);

}


TEST(TestLabo1_Etudiant, Supplementaire1_Mult_Vect_matrix)
{
    using namespace std::chrono;

    Vector<double, Dynamic> v;
    int size = 1000000;
    v.resize(size);

    for (int i = 0; i < size; ++i)
    {
        v(i) = i;
    }

    // MatRow * Vect  
    Matrix<double, Dynamic, Dynamic, RowStorage> M(100, size);
    M.setIdentity();

    high_resolution_clock::time_point t = high_resolution_clock::now();
    const auto b = M * v;
    const duration<double> optimal_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);
    std::cout << "(matRow*vect) time: " << optimal_t.count() << '\n';

    EXPECT_DOUBLE_EQ(b(0), 0.0);
    EXPECT_DOUBLE_EQ(b(1), 1.0);
    EXPECT_DOUBLE_EQ(b(2), 2.0);
    EXPECT_DOUBLE_EQ(b(3), 3.0);
    EXPECT_DOUBLE_EQ(b(4), 4.0);

    // MatCol * Vect
    Matrix<double, Dynamic, Dynamic, ColumnStorage> M2(100, size);
    M2.setIdentity();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    const auto b2 = M2 * v;
    const duration<double> optimal_t2 = duration_cast<duration<double>>(high_resolution_clock::now() - t2);
    std::cout << "(matCol*vect) time: " << optimal_t2.count() << '\n';

    EXPECT_DOUBLE_EQ(b2(0), 0.0);
    EXPECT_DOUBLE_EQ(b2(1), 1.0);
    EXPECT_DOUBLE_EQ(b2(2), 2.0);
    EXPECT_DOUBLE_EQ(b2(3), 3.0);
    EXPECT_DOUBLE_EQ(b2(4), 4.0);
}


TEST(TestLabo1_Etudiant, Supplementaire1_Mult_matrix_matrix_performance)
{
    using namespace std::chrono;
    int size = 750;

    Matrix<double, Dynamic, Dynamic, RowStorage> A_row(size, size);
    Matrix<double, Dynamic, Dynamic, ColumnStorage> B_col(size, size);
    Matrix<double, Dynamic, Dynamic, ColumnStorage> A_col(size, size);
    Matrix<double, Dynamic, Dynamic, RowStorage> B_row(size, size);

    // row * col
    {
        high_resolution_clock::time_point t = high_resolution_clock::now();

        auto C = A_row * B_col;

        const duration<double> naive_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);
        std::cout << "\nrow*col time: " << duration_cast<std::chrono::milliseconds>(naive_t).count() << " ms";
    }

    // col * row
    {
        high_resolution_clock::time_point t = high_resolution_clock::now();

        auto C = B_col * A_row;

        const duration<double> naive_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);
        std::cout << "\ncol*row time: " << duration_cast<std::chrono::milliseconds>(naive_t).count() << " ms";

    }

    // generic row*row
    {
        high_resolution_clock::time_point t = high_resolution_clock::now();

        auto C = A_row * B_row;

        const duration<double> naive_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);
        std::cout << "\nrow*row time: " << duration_cast<std::chrono::milliseconds>(naive_t).count() << " ms";
    }

    // generic col*col
    {
        high_resolution_clock::time_point t = high_resolution_clock::now();

        auto C = A_col * B_col;

        const duration<double> naive_t = duration_cast<duration<double>>(high_resolution_clock::now() - t);
        std::cout << "\ncol*col time: " << duration_cast<std::chrono::milliseconds>(naive_t).count() << " ms\n";
    }
}


#pragma endregion // Tests Supplémentaires
