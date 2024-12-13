#include "richtexthandler.h"

#include <QFile>
#include <QFileInfo>
#include <QFileSelector>
#include <QMimeDatabase>
#include <QQmlFile>
#include <QQmlFileSelector>
#include <QQuickTextDocument>
#include <QTextCharFormat>
//#include <QStringDecoder>
#include <QTextDocument>
#include <QDebug>

RichTextHandler::RichTextHandler(QObject *parent)
    : QObject(parent)
    , m_document(nullptr)
    , m_cursorPosition(-1)
    , m_selectionStart(0)
    , m_selectionEnd(0)
{
}

QQuickTextDocument *RichTextHandler::document() const
{
    return m_document;
}

void RichTextHandler::setDocument(QQuickTextDocument *document)
{
    if (document == m_document)
        return;

    if (m_document)
        m_document->textDocument()->disconnect(this);
    m_document = document;
    if (m_document)
        connect(m_document->textDocument(), &QTextDocument::modificationChanged, this, &RichTextHandler::modifiedChanged);
    emit documentChanged();
}

int RichTextHandler::cursorPosition() const
{
    return m_cursorPosition;
}

void RichTextHandler::setCursorPosition(int position)
{
    if (position == m_cursorPosition)
        return;

    m_cursorPosition = position;
    reset();
    emit cursorPositionChanged();
}

int RichTextHandler::selectionStart() const
{
    return m_selectionStart;
}

void RichTextHandler::setSelectionStart(int position)
{
    if (position == m_selectionStart)
        return;

    m_selectionStart = position;
    emit selectionStartChanged();
}

int RichTextHandler::selectionEnd() const
{
    return m_selectionEnd;
}

void RichTextHandler::setSelectionEnd(int position)
{
    if (position == m_selectionEnd)
        return;

    m_selectionEnd = position;
    emit selectionEndChanged();
}

QString RichTextHandler::fontFamily() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return QString();
    QTextCharFormat format = cursor.charFormat();
    return format.font().family();
}

void RichTextHandler::setFontFamily(const QString &family)
{
    QTextCharFormat format;
    format.setFontFamily(family);
    mergeFormatOnWordOrSelection(format);
    emit fontFamilyChanged();
}

QColor RichTextHandler::textColor() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return QColor(Qt::black);
    QTextCharFormat format = cursor.charFormat();
    return format.foreground().color();
}

void RichTextHandler::setTextColor(const QColor &color)
{
    QTextCharFormat format;
    format.setForeground(QBrush(color));
    mergeFormatOnWordOrSelection(format);
    emit textColorChanged();
}

Qt::Alignment RichTextHandler::alignment() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return Qt::AlignLeft;
    return textCursor().blockFormat().alignment();
}

void RichTextHandler::setAlignment(Qt::Alignment alignment)
{
    QTextBlockFormat format;
    format.setAlignment(alignment);
    QTextCursor cursor = textCursor();
    cursor.mergeBlockFormat(format);
    emit alignmentChanged();
}

bool RichTextHandler::bold() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontWeight() == QFont::Bold;
}

void RichTextHandler::setBold(bool bold)
{
    QTextCharFormat format;
    format.setFontWeight(bold ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(format);
    emit boldChanged();
}

bool RichTextHandler::italic() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontItalic();
}

void RichTextHandler::setItalic(bool italic)
{
    QTextCharFormat format;
    format.setFontItalic(italic);
    mergeFormatOnWordOrSelection(format);
    emit italicChanged();
}

bool RichTextHandler::underline() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontUnderline();
}

void RichTextHandler::setUnderline(bool underline)
{
    QTextCharFormat format;
    format.setFontUnderline(underline);
    mergeFormatOnWordOrSelection(format);
    emit underlineChanged();
}

int RichTextHandler::fontSize() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return 0;
    QTextCharFormat format = cursor.charFormat();
    return format.font().pointSize();
}

void RichTextHandler::setFontSize(int size)
{
    if (size <= 0)
        return;

    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;

    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    if (cursor.charFormat().property(QTextFormat::FontPointSize).toInt() == size)
        return;

    QTextCharFormat format;
    format.setFontPointSize(size);
    mergeFormatOnWordOrSelection(format);
    emit fontSizeChanged();
}

//QString RichTextHandler::fileName() const
//{
//    const QString filePath = QQmlFile::urlToLocalFileOrQrc(m_fileUrl);
//    const QString fileName = QFileInfo(filePath).fileName();
//    if (fileName.isEmpty())
//        return QStringLiteral("untitled.txt");
//    return fileName;
//}

//QString RichTextHandler::fileType() const
//{
//    return QFileInfo(fileName()).suffix();
//}

//QUrl RichTextHandler::fileUrl() const
//{
//    return m_fileUrl;
//}

//void RichTextHandler::load(const QUrl &fileUrl)
//{
//    if (fileUrl == m_fileUrl)
//        return;

//    QQmlEngine *engine = qmlEngine(this);
//    if (!engine) {
//        qWarning() << "load() called before RichTextHandler has QQmlEngine";
//        return;
//    }

//    const QUrl path = QQmlFileSelector::get(engine)->selector()->select(fileUrl);
//    const QString fileName = QQmlFile::urlToLocalFileOrQrc(path);
//    if (QFile::exists(fileName)) {
//        QMimeType mime = QMimeDatabase().mimeTypeForFile(fileName);
//        QFile file(fileName);
//        if (file.open(QFile::ReadOnly)) {
//            QByteArray data = file.readAll();
//            if (QTextDocument *doc = textDocument()) {
//                doc->setBaseUrl(path.adjusted(QUrl::RemoveFilename));
//                doc->setModified(false);
//                if (mime.inherits("text/markdown")) {
//                    emit loaded(QString::fromUtf8(data), Qt::MarkdownText);
//                } else {
//                    auto encoding = QStringConverter::encodingForHtml(data);
//                    if (encoding) {
//                        QStringDecoder decoder(*encoding);
//                        emit loaded(decoder(data), Qt::AutoText);
//                    } else {
//                        // fall back to utf8
//                        emit loaded(QString::fromUtf8(data), Qt::AutoText);
//                    }
//                }
//            }

//            reset();
//        }
//    }

//    m_fileUrl = fileUrl;
//    emit fileUrlChanged();
//}

//void RichTextHandler::saveAs(const QUrl &fileUrl)
//{
//    QTextDocument *doc = textDocument();
//    if (!doc)
//        return;

//    const QString filePath = fileUrl.toLocalFile();
//    const bool isHtml = QFileInfo(filePath).suffix().contains(QLatin1String("htm"));
//    QFile file(filePath);
//    if (!file.open(QFile::WriteOnly | QFile::Truncate | (isHtml ? QFile::NotOpen : QFile::Text))) {
//        emit error(tr("Cannot save: ") + file.errorString());
//        return;
//    }
//    file.write((isHtml ? doc->toHtml() : doc->toPlainText()).toUtf8());
//    file.close();

//    if (fileUrl == m_fileUrl)
//        return;

//    m_fileUrl = fileUrl;
//    emit fileUrlChanged();
//}

void RichTextHandler::reset()
{
    emit fontFamilyChanged();
    emit alignmentChanged();
    emit boldChanged();
    emit italicChanged();
    emit underlineChanged();
    emit fontSizeChanged();
    emit textColorChanged();
}

QTextCursor RichTextHandler::textCursor() const
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return QTextCursor();

    QTextCursor cursor = QTextCursor(doc);
    if (m_selectionStart != m_selectionEnd) {
        cursor.setPosition(m_selectionStart);
        cursor.setPosition(m_selectionEnd, QTextCursor::KeepAnchor);
    } else {
        cursor.setPosition(m_cursorPosition);
    }
    return cursor;
}

QTextDocument *RichTextHandler::textDocument() const
{
    if (!m_document)
        return nullptr;

    return m_document->textDocument();
}

void RichTextHandler::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
}

bool RichTextHandler::modified() const
{
    return m_document && m_document->textDocument()->isModified();
}

void RichTextHandler::setModified(bool m)
{
    if (m_document)
        m_document->textDocument()->setModified(m);
}

