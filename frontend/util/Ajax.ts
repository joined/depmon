import axios from 'axios';

export const getRequestSender = <TResponse>(url: string) => axios.get<TResponse>(url).then((res) => res.data);
export const postRequestSender = <TRequestBody>(url: string, { arg }: { arg: TRequestBody }) =>
    axios.post(url, { ...arg });
