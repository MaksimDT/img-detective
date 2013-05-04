#include "core/RDBMSImgMetadataStorage.h"
#include "utils/MemoryUtils.h"
#include "utils/UnicodeUtils.h"

using namespace ImgDetective::Core::Db;
using namespace std;

namespace ImgDetective {
namespace Core {

    CTOR RDBMSImgMetadataStorage::RDBMSImgMetadataStorage(REF Db::DbWrapper& dbWrapper) 
        : dbWrapper(dbWrapper) {
    }

    void RDBMSImgMetadataStorage::InitImgRecord(REF ImgInfo& imgInfo) const {
        char* utf8pathStr = NULL;
        size_t pathLengthUtf8 = 0;

        try {
            //prepare params BEGIN
            
            params_list_t params;

            //path
            Utils::Unicode::ToUtf8cstr(imgInfo.GetPath().wstring(), OUT utf8pathStr, OUT pathLengthUtf8);
            DbParamBuffer path(utf8pathStr, pathLengthUtf8, DbType::VARCHAR);
            params.push_back(path);

            //file extension
            string extension = imgInfo.GetFileExtension();
            DbParamBuffer extensionParam = DbParamBuffer::CreateNull(DbType::VARCHAR);
            if (!extension.empty()) {
                extensionParam = DbParamBuffer((void*)extension.c_str(), extension.length(), DbType::VARCHAR);
            }
            params.push_back(extensionParam);

            //repository id
            DbParamBuffer repoIdParam = DbParamBuffer::CreateNull(DbType::LONG);
            repoid_t repoId = imgInfo.GetRepoId();
            if (repoId != REPO_ID_UNDEFINED) {
                repoIdParam = DbParamBuffer(&repoId, sizeof(repoId), DbType::LONG);
            }
            params.push_back(repoIdParam);

            //prepare params END

            NonQueryExecResult dbResult = dbWrapper.ExecuteNonQuery(
                "INSERT INTO Images \
                 (Path, Extension, RepositoryId, CreationDate, Description) \
                 VALUES \
                 (?, ?, ?, NOW(), NULL)", params);

            imgid_t generatedImgId = dbResult.GetLastInsertId();
            imgInfo.SetId(generatedImgId);

            Utils::Memory::SafeDeleteArray(utf8pathStr);
        }
        catch (...) {
            Utils::Memory::SafeDeleteArray(utf8pathStr);
            throw;
        }
    }

    FsRepositoryInfo RDBMSImgMetadataStorage::CreateFsRepositoryRecord(const boost::filesystem::path& dirPath) const {
        char* utf8path = NULL;
        size_t utf8bufLength = 0;

        try {
            params_list_t params;
            
            Utils::Unicode::ToUtf8cstr(dirPath.wstring(), OUT utf8path, OUT utf8bufLength);
            DbParamBuffer pathParam(utf8path, utf8bufLength, DbType::VARCHAR);
            params.push_back(pathParam);

            NonQueryExecResult result = dbWrapper.ExecuteNonQuery(
                "INSERT INTO Repositories \
                (Path) \
                VALUES \
                (?)", params);

            //NOTE: conversion!
            repoid_t newRepoId = result.GetLastInsertId();
            FsRepositoryInfo repo(dirPath, newRepoId);

            Utils::Memory::SafeDeleteArray(utf8path);

            return repo;
        }
        catch (...) {
            Utils::Memory::SafeDeleteArray(utf8path);
            throw;
        }
    }

    FsRepositoryInfo::col_t RDBMSImgMetadataStorage::GetAllRepositories() const {
        DbResultReader* reader = NULL;

        try {
            reader = dbWrapper.ExecuteReader(
                "SELECT \
                    Id, Path \
                 FROM \
                    Repositories");

            FsRepositoryInfo::col_t result;

            while (reader->Next()) {
                repoid_t repoId = reader->GetField("Id").As<repoid_t>();
                wstring path = reader->GetField("Path").FromUtf8();
                FsRepositoryInfo repo(path, repoId);
                result.push_back(repo);
            }

            Utils::Memory::SafeDelete(reader);

            return result;
        }
        catch (...) {
            Utils::Memory::SafeDelete(reader);
            throw;
        }
    }
}
}