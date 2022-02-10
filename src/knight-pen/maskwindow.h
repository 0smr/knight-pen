#pragma once

#include <QQuickPaintedItem>
#include <QQuickWindow>
#include <QQuickItem>
#include <QPainter>
#include <QBitmap>
#include <QPixmap>
#include <QRegion>
#include <QRgb>

using booleanMatrix = std::vector<std::vector<bool>>;

class tileMaskWindow : public QQuickWindow {
    Q_OBJECT
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)
    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY columnsChanged)
    Q_PROPERTY(TransposeType transposed READ transposed WRITE setTransposed NOTIFY transposedChanged)
public:
    enum TransposeType {
        None,
        PrimaryTranspose,
        SecondaryTranspose,
    };
    Q_ENUM(TransposeType)

    tileMaskWindow()
        : mRows(1),
          mColumns(1),
          mTileSize(QSize(width() / mRows, height() / mColumns)),
          mTransposed(TransposeType::None),
          mMaskMatrix(booleanMatrix(1, std::vector<bool>(1, false))) {
        setFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Widget);
        setColor(Qt::transparent);
    }

    void resizeEvent(QResizeEvent *ev) override {
        mTileSize.setWidth(width()/mColumns);
        mTileSize.setHeight(height()/mRows);

        updateMask();
        QQuickWindow::resizeEvent(ev);
    }

    int rows() const { return mRows; }
    int columns() const { return mColumns; }
    TransposeType transposed() const { return mTransposed; }

    void setRows(int newRows) {
        if(mRows == newRows)
            return;
        mRows = newRows;

        mMaskMatrix.resize(mRows, std::vector<bool>(mColumns, false));
        mTileSize.setHeight(height()/mRows);

        emit rowsChanged();
        updateMask();
    }

    void setColumns(int newColumns) {
        if(mColumns == newColumns)
            return;
        mColumns = newColumns;

        for(auto &row : mMaskMatrix)
            row.resize(mColumns, false);
        mTileSize.setWidth(width()/mColumns);

        emit columnsChanged();
        updateMask();
    }

    void setTransposed(TransposeType newTransposed) {
        if(mTransposed == newTransposed)
            return;
        mTransposed = newTransposed;

        emit transposedChanged();
        updateMask();
    }

public slots:
    void updateMask() {
        QRegion region;
        booleanMatrix mask = mMaskMatrix;

        if(mTransposed != TransposeType::None) {
            mask = transpose(mMaskMatrix, mTransposed == TransposeType::SecondaryTranspose);
        }

        /// * matrix row/col may change after transpose. (not in this program)
        int row = mask.size(), column = mask[0].size();

        for(int i = 0; i < row; ++i) {
            for(int j = 0 ; j < column; ++j) {
                if(mask[i][j] == true) {
                    QPoint pos(j * mTileSize.width(), i * mTileSize.height());
                    region = region.united(QRect(pos - QPoint(2, 2), mTileSize + QSize(4, 4)));
                }
            }
        }

        setMask(region);
        update();
    }

    void setMaskArea(int i, int j, int i2, int j2, bool val = true) {
        if(0 <= i && 0 <= j && i2 < mRows && j2 < mColumns) {
            for(; i <= i2; ++i) {
                for(int k = j; k <= j2; ++k) {
                    mMaskMatrix[i][k] = val;
                }
            }
            updateMask();
        }
    }

    void toggleMaskArea(int i, int j, int i2, int j2) {
        if(0 <= i && 0 <= j && i2 < mRows && j2 < mColumns) {
            for(; i < i2; ++i) {
                for(; j < j2; ++j) {
                    mMaskMatrix[i][j] = !mMaskMatrix[i][j];
                }
            }
            updateMask();
        }
    }

    booleanMatrix transpose(booleanMatrix matrix, bool secondaryDiagonal = true) {
        if(mRows != mColumns)/// FIXME: handle mRow != mColumn.
            return matrix;
        for(int i = 0; i < mRows; ++i) {
            for(int j = 0 ; j < mColumns; ++j) {
                if(secondaryDiagonal == true && i + j < std::min(mRows, mColumns)) {
                    std::swap(matrix[mColumns - j - 1][mRows - i - 1], matrix[i][j]);
                } else if(secondaryDiagonal == false && j < i) {
                    std::swap(matrix[j][i], matrix[i][j]);
                }
            }
        }
        return matrix;
    }

    /**
     * NOTE: transparent mouse & keyboard input on specific spots.
     * https://stackoverflow.com/questions/20401896/qt-global-mouse-listener
     * https://stllug.sluug.org/meeting_notes/1997/0619/xkey.html
     * https://stackoverflow.com/questions/8629474/how-to-globally-capture-every-mouse-click-in-x11
     */
signals:
    void rowsChanged();
    void columnsChanged();
    void transposedChanged();

private:
    int mRows, mColumns;
    QSize mTileSize;
    TransposeType mTransposed;
    booleanMatrix mMaskMatrix;
};
